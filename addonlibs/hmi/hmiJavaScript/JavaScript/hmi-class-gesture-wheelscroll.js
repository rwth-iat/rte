/*
*   Copyright (C) 2008
*   Lehrstuhl fuer Prozessleittechnik,
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
*	$Revision$
*	$Date$
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
		//Hide addEventListener from nativ IE
		if("unknown" == typeof Component.addEventListener || Component.addEventListener){
			//mozilla
			Component.addEventListener('DOMMouseScroll', this._onScrollThunk, capture); 
			//opera
			Component.addEventListener('mousewheel', this._onScrollThunk, capture);
			//Adobe SVG Viewer has no wheelsupport
			//Renesis unknown
		}
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
		var Component = Scrollframe.firstChild;
		while (	Component !== null && HMI.instanceOf(Component, "hmi-component-scrollcontent") === false){
			Component = Component.nextSibling;
		}

		if(!HMI.currY){
			HMI.currY = parseInt(Component.getAttribute('y'),10);
			HMI.scrollComponent = Component.id;
		}
		
		//HMI.currY = eval(HMI.currY - wheelData * 4);
		var y = parseInt(wheelData * 4,10);
		if(parseInt(Component.getAttribute('height'),10)>parseInt(Scrollframe.getAttribute('height'),10)){
			if (HMI.currY + y + parseInt(Component.getAttribute('height'),10) < parseInt(Scrollframe.getAttribute('height'),10)){
				HMI.currY = parseInt(Scrollframe.getAttribute('height'),10) - parseInt(Component.getAttribute('height'),10);
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
		while (	Component !== null && HMI.instanceOf(Component, "hmi-component-scrollcontent") === false){
			Component = Component.nextSibling;
		}

		if(!HMI.currY){
			HMI.currY = parseInt(Component.getAttribute('y'),10);
			HMI.scrollComponent = Component.id;
		}
		if(!HMI.currX){
			HMI.currX = parseInt(Component.getAttribute('x'),10);
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
		if(parseInt(Component.getAttribute('height'),10)>parseInt(Scrollframe.getAttribute('height'),10)){
			if (HMI.currY + y + parseInt(Component.getAttribute('height'),10) < parseInt(Scrollframe.getAttribute('height'),10)){
				HMI.currY = parseInt(Scrollframe.getAttribute('height'),10) - parseInt(Component.getAttribute('height'),10);
			} else if(HMI.currY + y > 0){
				HMI.currY = 0;
			} else {
				HMI.currY += y;
			}
		} else {
			HMI.currY = 0;
		}
		
		//calculate  x
		if(parseInt(Component.getAttribute('width'),10)>parseInt(Scrollframe.getAttribute('width'),10)){
			if (HMI.currX + x + parseInt(Component.getAttribute('width'),10) < parseInt(Scrollframe.getAttribute('width'),10)){
				HMI.currX = parseInt(Scrollframe.getAttribute('width'),10) - parseInt(Component.getAttribute('width'),10);
			} else if (HMI.currX + x > 0){
				HMI.currX = 0;
			} else {
				HMI.currX += x;
			}
		} else {
			HMI.currX = 0;
		}
		Component.setAttribute("y", HMI.currY);
		Component.setAttribute("x", HMI.currX);
		
		return;
	}
};
var filedate = "$Date$";
filedate = filedate.substring(7, filedate.length-2);
if ("undefined" == typeof HMIdate){
	HMIdate = filedate;
}else if (HMIdate < filedate){
	HMIdate = filedate;
}
