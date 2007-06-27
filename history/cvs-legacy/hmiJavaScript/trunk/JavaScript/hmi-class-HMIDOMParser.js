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
*	hmi-class-HMIDOMParser.js
*
*	Editors:
*	--------
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision: 1.1.1.1 $
*	$Date: 2007-06-27 15:37:43 $
*
*	History:
*	--------
*	21-June-2007			St
*		-	General Revision
*
***********************************************************************/

/***********************************************************************
	constructor
***********************************************************************/

function HMIDOMParser() {
};

/***********************************************************************
	prototype
***********************************************************************/

HMIDOMParser.prototype = {
	
	/*********************************
		parse
	*********************************/
	parse: function(GraphicDescription, StyleDescription, BehaviourDescription) {
		var Parser = new DOMParser();
		var GraphicElement;
		var StyleElement;
		var Return;
		
		try {
			GraphicElement = Parser.parseFromString(GraphicDescription, "text/xml");
		} catch (e) {
			HMI.hmi_log_error('HMIDOMParser.prototype.parse: Could not parse GraphicDescription');
			return null;
		};
		if (GraphicElement.documentElement.namespaceURI == "http://www.mozilla.org/newlayout/xml/parsererror.xml")
		{
			HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on GraphicDescription');
			return null;
		};
			
		try {
			StyleElement = Parser.parseFromString(StyleDescription, "text/xml");
		} catch (e) {
			HMI.hmi_log_error('HMIDOMParser.prototype.parse: Could not parse StyleDescription');
			return null;
		};
		if (StyleElement.documentElement.namespaceURI != "http://www.mozilla.org/newlayout/xml/parsererror.xml")
		{	
			GraphicElement.firstChild.appendChild(StyleElement.firstChild);
		} else {
			HMI.hmi_log_error('HMIDOMParser.prototype.parse: ParseError on StyleDescription');
		};
		
		Return = GraphicElement.firstChild;
	   
	   delete Parser;
	   delete GraphicElement;
	   delete StyleElement;
		
		return Return;
	}
};