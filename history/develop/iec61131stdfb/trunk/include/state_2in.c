/*
*	Copyright (C) 2010
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*/
/***********************************************************************
*
*	File
*	-----
*	state_2in.c
*
*	Autoren:
*	--------
*	LE						Lars Evertz <l.evertz@plt.rwth-aachen.de>
*
*	
*	Historie:
*	--------
*	14-December-2010		LE
*		-	file created
*
***********************************************************************/

		//	states won't be passed on through blocks
	/*	if((pinst->v_IN1.value.vartype & OV_VT_HAS_STATE)
			&& (pinst->v_IN2.value.vartype & OV_VT_HAS_STATE))		//TRUE if both inputs' STATE FLAGs are set
		{
			pinst->v_OUT.value.vartype = pinst->v_OUT.value.vartype | OV_VT_HAS_STATE;
			if(pinst->v_IN1.state == pinst->v_IN2.state)			//if both states are the same, set output state (GOOD states are handled here)
				pinst->v_OUT.state = pinst->v_IN1.state;
			if((pinst->v_IN1.state == OV_ST_BAD) 
				|| (pinst->v_IN2.state == OV_ST_BAD))				//output BAD if only one value is marked BAD
				pinst->v_OUT.state = OV_ST_BAD;
			else
				if((pinst->v_IN1.state == OV_ST_QUESTIONABLE)
					|| (pinst->v_IN2.state == OV_ST_QUESTIONABLE))	//if not BAD check better states in same scheme
					pinst->v_OUT.state = OV_ST_QUESTIONABLE;
					else
						if((pinst->v_IN1.state == OV_ST_UNKNOWN)
							|| (pinst->v_IN2.state == OV_ST_UNKNOWN))
							pinst->v_OUT.state = OV_ST_UNKNOWN;
						else
							if((pinst->v_IN1.state == OV_ST_NOTSUPPORTED)
								|| (pinst->v_IN2.state == OV_ST_NOTSUPPORTED))	//if HAS_STATE is set, though state is NOT_SUPPORTED (only PVs should behave this way), output state is UNKNOWN
								pinst->v_OUT.state = OV_ST_UNKNOWN;			//STATE is GOOD, if both variables are marked GOOD, case is handled above
						
		}
		else
		{
			if(pinst->v_IN1.value.vartype & OV_VT_HAS_STATE)		//if only IN1 has a set STATE FLAG the state is copied to OUTPUT
			{
				pinst->v_OUT.value.vartype = pinst->v_OUT.value.vartype | OV_VT_HAS_STATE;
				pinst->v_OUT.state = pinst->v_IN1.state;
			}
			else
			if(pinst->v_IN2.value.vartype & OV_VT_HAS_STATE)		//same for IN2
			{
				pinst->v_OUT.value.vartype = pinst->v_OUT.value.vartype | OV_VT_HAS_STATE;
				pinst->v_OUT.state = pinst->v_IN2.state;

			}
		}			
		*/
		
#ifdef STDFB_STATE_CHECK
	if(STDFB_bad_operation)
	{
		pinst->v_OUT.value.vartype = pinst->v_OUT.value.vartype | OV_VT_HAS_STATE;
		pinst->v_OUT.state = OV_ST_BAD;
	}
#endif
		
		
		
		if((pinst->v_IN1.value.vartype & OV_VT_HAS_TIMESTAMP)			//if both inputs have a timestamp
			&& (pinst->v_IN2.value.vartype & OV_VT_HAS_TIMESTAMP))
		{
			pinst->v_OUT.value.vartype = pinst->v_OUT.value.vartype | OV_VT_HAS_TIMESTAMP;
			if((pinst->v_IN1.time.secs < pinst->v_IN2.time.secs)
				|| ((pinst->v_IN1.time.secs == pinst->v_IN2.time.secs) && (pinst->v_IN1.time.usecs < pinst->v_IN2.time.usecs)))	//if IN1 older than IN2
			{
				pinst->v_OUT.time.secs = pinst->v_IN1.time.secs;
				pinst->v_OUT.time.usecs = pinst->v_IN1.time.usecs;
			}
			else
			{
				if((pinst->v_IN2.time.secs < pinst->v_IN1.time.secs)
					|| ((pinst->v_IN2.time.secs == pinst->v_IN1.time.secs) && (pinst->v_IN2.time.usecs < pinst->v_IN1.time.usecs)))	//if IN2 older than IN1
				{
					pinst->v_OUT.time.secs = pinst->v_IN2.time.secs;
					pinst->v_OUT.time.usecs = pinst->v_IN2.time.usecs;
					
				}
				else		//same time
				{
					pinst->v_OUT.time.secs = pinst->v_IN1.time.secs;
					pinst->v_OUT.time.usecs = pinst->v_IN1.time.usecs;
					
				}
			}
			
		}
		else
		{
			if(pinst->v_IN1.value.vartype & OV_VT_HAS_TIMESTAMP)	//only IN1 has a timestamp
			{
				pinst->v_OUT.value.vartype = pinst->v_OUT.value.vartype | OV_VT_HAS_TIMESTAMP;
				pinst->v_OUT.time.secs = pinst->v_IN1.time.secs;
				pinst->v_OUT.time.usecs = pinst->v_IN1.time.usecs;
				
			}
			else
			{
				if(pinst->v_IN2.value.vartype & OV_VT_HAS_TIMESTAMP)		//only IN1 has a timestamp
				{
					pinst->v_OUT.value.vartype = pinst->v_OUT.value.vartype | OV_VT_HAS_TIMESTAMP;
					pinst->v_OUT.time.secs = pinst->v_IN2.time.secs;
					pinst->v_OUT.time.usecs = pinst->v_IN2.time.usecs;
					
				}
			}
		}
		
		