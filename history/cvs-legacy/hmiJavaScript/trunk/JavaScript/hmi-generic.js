/*
*	Copyright (C) 2007
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
*	File:
*	------
*	hmi-generic.js
*
*	Editors:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	Je							Holger Jeromin <Holger.Jeromin@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.4 $
*	$Date: 2008-03-28 09:33:41 $
*
*	History:
*	--------
*	21-June-2007			St
*		-	General Revision
*
*	26-March-2008			Je
*		-	Header hidable
*
************************************************************************/

/*********************************
	Global Variables
*********************************/

var HMI = new HMI(false, true, true, true, true, false);

/*********************************
	Functions - $
*********************************/

function $(id) {
	return document.getElementById(id)
}

/*********************************
	Functions - deleteChilds
*********************************/

function deleteChilds(ParentNode) {
	HMI.hmi_log_trace("deleteChilds - Start");
	
	while (ParentNode.childNodes.length > 0)
		ParentNode.removeChild(ParentNode.lastChild);
	
	HMI.hmi_log_trace("deleteChilds - End");
}

/*********************************
	Functions - hideHeader
*********************************/
function hideHeader (){
	if (showHeader){
		//menue ausschalten
		showHeader = false;
		document.getElementById("hmi_header").style.display = "none";
		document.getElementById("knopftext").innerHTML = "HMI-Header einblenden";
		document.getElementById("pfeile").innerHTML = "&#8659;";
	} else {
		//menue einschalten
		showHeader = true;
		document.getElementById("hmi_header").style.display = "block";
		document.getElementById("knopftext").innerHTML = "HMI-Header ausblenden";
		document.getElementById("pfeile").innerHTML = "&#8657;";
	}
}