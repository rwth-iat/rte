/*
*   $Id: ov_supervise.c,v 1.1 1999-07-19 15:02:14 dirk Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	16-Dec-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_supervise.h"

#include <setjmp.h>
#if OV_SYSTEM_UNIX
#include <sys/time.h>
#include <signal.h>
#elif OV_SYSTEM_NT
#include <windows.h>
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Global variables
*/
static OV_JUMPBUFFER	*pjmpbuf;
static OV_BOOL			supervise = FALSE;
#if OV_SYSTEM_NT
static UINT				timeractive;
static HWND				timerwindow = NULL;
static HANDLE			timerwindowstarted;
static HANDLE			mainthread = NULL;
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Signalhandler under Unix
*/
#if OV_SYSTEM_UNIX
void ov_supervise_signalhandler(int sig) {
	if(ov_supervise_end()) {
		siglongjmp(*pjmpbuf, 1);
	}
}
#endif

/*	----------------------------------------------------------------------	*/

#if OV_SYSTEM_NT
/*
*	Execute LongJump under Windows NT
*/
void ov_supervise_execlongjump(void) {
	longjmp(*pjmpbuf, 1);
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Timer method of the time window under Windows NT
*/
#if OV_SYSTEM_NT
LRESULT CALLBACK ov_supervise_timerwndproc(HWND hWnd, UINT Msg,
	WPARAM wParam, LPARAM lParam
) {
	switch(Msg) {
		case WM_TIMER:
			if(wParam == timeractive) {
				if(ov_supervise_end()) {
					CONTEXT context;
					SuspendThread(mainthread);
					context.ContextFlags = CONTEXT_CONTROL;
					if(GetThreadContext(mainthread, &context)) {
						context.Eip = (DWORD)ov_supervise_execlongjump;
						SetThreadContext(mainthread, &context);
					}
					ResumeThread(mainthread);
				}
			}
			return 0;
		default:
			break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	WinMain method of the timer window unter Windows NT
*/
#if OV_SYSTEM_NT
DWORD ov_supervise_winmain() {
	MSG msg;
	WNDCLASS wc;
	static char classname[] = "OvTimerWindowClass";
	/*
	*	register timer window class
	*/
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)ov_supervise_timerwndproc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(0);
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = classname;
	if(!RegisterClass(&wc)) {
		return FALSE;
	}
	/*
	*	created invisible timer window
	*/
	timerwindow = CreateWindow(classname, classname, WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		(HWND)NULL, (HMENU)NULL, GetModuleHandle(0), (LPVOID)NULL);
	SetEvent(timerwindowstarted);
	if(!timerwindow) {
		return FALSE;
	}
	/*
	*	start the message loop
	*/
	while(GetMessage(&msg, timerwindow, 0, 0)) {
		DispatchMessage(&msg);
	}
	/*
	*	finished
	*/
	return msg.wParam;
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Start the supervision of a user function
*/
OV_BOOL OV_DLLFNCEXPORT ov_supervise_start(
	OV_TIME_SPAN	*ptimeout,
	OV_JUMPBUFFER	*pjumpbuffer
) {
	/*
	*	local variables
	*/
#if OV_SYSTEM_UNIX
	struct itimerval it;
#elif OV_SYSTEM_NT
	UINT timeout;
#endif
	if(supervise || (!pjumpbuffer)) {
		return FALSE;
	}
	pjmpbuf = pjumpbuffer;
#if OV_SYSTEM_UNIX
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 0;
	it.it_value.tv_sec = ptimeout->secs;
	it.it_value.tv_usec = ptimeout->usecs;
	if(setitimer(ITIMER_REAL, &it, NULL)) {
		return FALSE;
	}
	signal(SIGALRM, ov_supervise_signalhandler);
	supervise = TRUE;
	return TRUE;
#elif OV_SYSTEM_NT
	if(!timerwindow) {
		/*
		*	there's no timer window yet; create a new thread
		*	for the timer window
		*/
		HANDLE thread;
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = FALSE;
		sa.lpSecurityDescriptor = NULL;
		timerwindowstarted = CreateEvent(&sa, TRUE, FALSE, NULL);
		thread = CreateThread(&sa, 0, (LPTHREAD_START_ROUTINE)
			ov_supervise_winmain, NULL, 0, NULL);
		if(!thread) {
			return FALSE;
		}
		SetThreadPriority(thread, THREAD_PRIORITY_HIGHEST);
		CloseHandle(thread);
		WaitForSingleObject(timerwindowstarted, INFINITE);
		CloseHandle(timerwindowstarted);
		if(!timerwindow) {
			return FALSE;
		}
	}
	if(!mainthread) {
		/*
		*	the handle of the calling thread is still unknown 
		*/
		if(!DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
			GetCurrentProcess(), &mainthread, 0, FALSE,
			DUPLICATE_SAME_ACCESS)
		) {
			return FALSE;
		}
		if(!mainthread) {
			return FALSE;
		}
	}
	if(timeractive) {
		KillTimer(timerwindow, timeractive);
		timeractive = 0;
	}
	timeout = ptimeout->secs*1000+ptimeout->usecs/1000;
	if(!timeout) {
		return FALSE;
	}
	timeractive = SetTimer(timerwindow, 1, timeout, NULL);
	if(!timeractive) {
		return FALSE;
	}
	supervise = TRUE;
	return TRUE;
#else
	supervise = TRUE;
	return TRUE;
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Finish supervising a user function
*/
OV_BOOL OV_DLLFNCEXPORT ov_supervise_end(void) {
	/*
	*	local variables
	*/	
#if OV_SYSTEM_UNIX
	struct itimerval it;
#endif
	if(!supervise) {
		return FALSE;
	}
#if OV_SYSTEM_UNIX
	signal(SIGALRM, SIG_IGN);
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 0;
	it.it_value.tv_sec = 0;
	it.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &it, NULL);
	supervise = FALSE;
	return TRUE;
#elif OV_SYSTEM_NT
	supervise = FALSE;
	return TRUE;
#else
	supervise = FALSE;
	return TRUE;
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

