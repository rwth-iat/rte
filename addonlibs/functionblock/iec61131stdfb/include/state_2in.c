/*
*   Copyright (C) 2010
*   Chair of Process Control Engineering,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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
		
		