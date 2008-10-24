/*
*	Copyright (C) 2008
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
*	hmi-class-gesture-wheelscroll.js
*
*	Editors:
*	--------
*	NH							Nikolas Hansen <Nikolas.Hansen@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	
*	$Revision: 1.2 $
*	$Date: 2008-10-24 12:49:42 $
*
*	History:
*	--------
*
***********************************************************************/

//wheelsupport is not supported by the HMI Team and probably firefox only

/***********************************************************************
	constructor
***********************************************************************/

function WheelScroll (Component, Controller) {
	this.arm(Component);
};

/***********************************************************************
	prototype
***********************************************************************/

WheelScroll.prototype = {
	/*********************************
		arm
	*********************************/
	arm : function (Component) {
		this._registerOnScroll(Component, false, this);
	},
	
	/*********************************
		_registerOnScroll
	*********************************/
	_registerOnScroll: function(Component, capture, listener) {
		this._onScrollThunk = function (evt) { listener.onScroll(evt); };
			//mozilla
			Component.addEventListener('DOMMouseScroll', this._onScrollThunk, capture); 
			//opera
			Component.addEventListener('mousewheel', this._onScrollThunk, capture);
			//Adobe SVG Viewer has no wheelsupport
	},
	
	/*********************************
		onScroll
	*********************************/
	_onScrollThunk: null,
	onScroll: function (evt) {
		evt = evt ? evt : window.event;
		//evt.stopPropagation();	
		var wheelData = evt.detail ? evt.detail * -1 : evt.wheelDelta / 40;
		//get and translate parent svg object
		var Scrollframe = HMI.getComponent(evt, 'hmi-component-scrollframe');
		//alert(Scrollframe.id);
		var Component = Scrollframe.firstChild;
		while (	Component != null && HMI.instanceOf(Component, "hmi-component-scrollcontent") == false){
			Component = Component.nextSibling;
		}

		if(!HMI.currY){
			HMI.currY = parseInt(Component.getAttribute('y'));
			HMI.scrollComponent = Component.id;
		}
		
		//HMI.currY = eval(HMI.currY - wheelData * 4);
		var y = parseInt(wheelData * 4);
		if(parseInt(Component.getAttribute('height'))>parseInt(Scrollframe.getAttribute('height'))){
			if (HMI.currY + y + parseInt(Component.getAttribute('height')) < parseInt(Scrollframe.getAttribute('height'))){
				HMI.currY = parseInt(Scrollframe.getAttribute('height')) - parseInt(Component.getAttribute('height'));
			} else if(HMI.currY + y > 0){
				HMI.currY = 0;
			} else {
				HMI.currY += y;
			}
		} else {
			HMI.currY = 0;
		}
		Component.setAttribute("y", HMI.currY);
		
		return this.cancelScroll(evt);
	},
	
	
	cancelScroll: function (evt) {
		evt = evt ? evt : window.event;
		if(evt.stopPropagation)
			evt.stopPropagation();
		if(evt.preventDefault)
			evt.preventDefault();
		evt.cancelBubble = true;
		evt.cancel = true;
		evt.returnValue = false;
		return false;
	},
	
	scroll: function (evt, orientation){
		evt = evt ? evt : window.event;
		var Scrollframe = HMI.getComponent(evt, 'hmi-component-scrollframe');
		var Component = Scrollframe.firstChild;
		while (	Component != null && HMI.instanceOf(Component, "hmi-component-scrollcontent") == false){
			Component = Component.nextSibling;
		}

		if(!HMI.currY){
			HMI.currY = parseInt(Component.getAttribute('y'));
			HMI.scrollComponent = Component.id;
		}
		if(!HMI.currX){
			HMI.currX = parseInt(Component.getAttribute('x'));
			HMI.scrollComponent = Component.id;
		}
		
		var x = 0;
		var y = 0;
		switch(orientation){
		case 1:
			x = 30;
			break;
		case 2:
			x = -30;
			break;
		case 3:
			y = 30;
			break;
		case 4:
			y = -30;
			break;
		default:
			break;
		}
		
		//calculate y
		if(parseInt(Component.getAttribute('height'))>parseInt(Scrollframe.getAttribute('height'))){
			if (HMI.currY + y + parseInt(Component.getAttribute('height')) < parseInt(Scrollframe.getAttribute('height'))){
				HMI.currY = parseInt(Scrollframe.getAttribute('height')) - parseInt(Component.getAttribute('height'));
			} else if(HMI.currY + y > 0){
				HMI.currY = 0;
			} else {
				HMI.currY += y;
			}
		} else {
			HMI.currY = 0;
		}
		
		//calculate  x
		if(parseInt(Component.getAttribute('width'))>parseInt(Scrollframe.getAttribute('width'))){
			if (HMI.currX + x + parseInt(Component.getAttribute('width')) < parseInt(Scrollframe.getAttribute('width'))){
				HMI.currX = parseInt(Scrollframe.getAttribute('width')) - parseInt(Component.getAttribute('width'));
			} else if (HMI.currX + x > 0){
				HMI.currX = 0;
			} else {
				HMI.currX += x;
			}
		} else {
			HMI.currX = 0;
		}
		//alert(HMI.currX);
		//alert(x);
		//alert(parseInt(Component.getAttribute('width')));
		//alert(parseInt(Scrollframe.getAttribute('width')));
		Component.setAttribute("y", HMI.currY);
		Component.setAttribute("x", HMI.currX);
		
		return;
	}
};
