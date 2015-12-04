#include <signal.h>
#include <windows.h>

class QuitMsgWnd {
public:
	QuitMsgWnd(const char *wndname);
	~QuitMsgWnd();

private:
	static DWORD wndthread(DWORD);
	static LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	static const char	*_wndname;
	static HANDLE		_wndstarted_event;
	static HANDLE		_thread;
	static WNDCLASS		_wndclass;
	static QuitMsgWnd	*_the_quitmsgwnd;
};

// Static data members

const char	*QuitMsgWnd::_wndname;
HANDLE		QuitMsgWnd::_wndstarted_event;
HANDLE		QuitMsgWnd::_thread;
WNDCLASS	QuitMsgWnd::_wndclass;
QuitMsgWnd	*QuitMsgWnd::_the_quitmsgwnd = 0;

// Constructor

QuitMsgWnd::QuitMsgWnd(const char *wndname) {
	static char	*wndclassname = "AcpltQuitMessageWindowClass";
	if(!_the_quitmsgwnd) {
		_wndname = wndname;
		_wndstarted_event = 0;
		_thread = 0;
		_wndclass.style = 0;
		_wndclass.lpfnWndProc = (WNDPROC)wndproc;
		_wndclass.cbClsExtra = 0;
		_wndclass.cbWndExtra = 0;
		_wndclass.hInstance = GetModuleHandle(0);
		_wndclass.hIcon = NULL;
		_wndclass.hCursor = NULL;
		_wndclass.hbrBackground = NULL;
		_wndclass.lpszMenuName = NULL;
		_wndclass.lpszClassName = wndclassname;
		if(!RegisterClass(&_wndclass)) {
			return;
		}
		_wndstarted_event = CreateEvent(NULL, FALSE, FALSE, NULL);
		if(!_wndstarted_event) {
			return;
		}
		_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)wndthread, NULL, 0, NULL);
		if(!_thread) {
			return;
		}
		WaitForSingleObject(_wndstarted_event, INFINITE);
		_the_quitmsgwnd = this;
	}
}

// Destructor

QuitMsgWnd::~QuitMsgWnd() {
	if(_the_quitmsgwnd == this) {
		if(_wndstarted_event) {
			CloseHandle(_wndstarted_event);
		}
		if(_thread) {
			TerminateThread(_thread, 0);
			CloseHandle(_thread);
		}
		UnregisterClass(_wndclass.lpszClassName, _wndclass.hInstance);
		_the_quitmsgwnd = 0;
	}
}

// Window thread

DWORD
QuitMsgWnd::wndthread(DWORD) {
	MSG		msg;
	HWND	wnd;
	wnd = CreateWindow(_wndclass.lpszClassName, _wndname, WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, GetModuleHandle(0), NULL);
	SetEvent(_wndstarted_event);
	if(!wnd) {
		return FALSE;
	}
	/*
	*	start the message loop
	*/
	while(GetMessage(&msg, wnd, 0, 0)) {
		DispatchMessage(&msg);
	}
	/*
	*	finished
	*/
	return msg.wParam;
}

// Window procedure

LRESULT CALLBACK
QuitMsgWnd::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch(msg) {
	case WM_ENDSESSION:
		raise(SIGTERM);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

static QuitMsgWnd quitmsgwnd("ovserver_demo");

