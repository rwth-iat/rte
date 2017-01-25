var Submitbutton = document.getElementById("idSubmitbutton");
var RefreshButton = document.getElementById("idRefresh");
var RequestGetURL	= document.getElementById("idGetRequestURL");
var RequestPostURL	= document.getElementById("idPostRequestURL");
var RequestType = document.getElementById("idrequestType");
var RequestTypeSelect = document.getElementById("idrequestTypeSelect");
var UrlInput = document.getElementById("idUrlInput");
var UrlBodyInput = document.getElementById("idBodyInput");
var PlaygroundOutput = document.getElementById("idPlaygroundOutput");
var ServerList = document.getElementById("idServersSelect");
var FieldSetGET = document.getElementById("idGETFieldset");
var FieldSetPOST = document.getElementById("idPOSTFieldset");
var FieldSetMSG = document.getElementById("idMSGFieldset");
var FieldSetSVC = document.getElementById("idSVCFieldset");
var MsgRcvAddress = document.getElementById("idMsgReceiverLocalAddress");
var MsgBody = document.getElementById("idMsgBodyInput");
var SvcRcvAddress = document.getElementById("idSvcReceiverLocalAddress");
var SvcCallSvc = document.getElementById("idSvcSvcName");
var SvcCallOp = document.getElementById("idSvcOperationName");
var SvcCallBody = document.getElementById("idSVCBodyInput");

var HostUrl;
var HostName;
var ServerInstances;
var ServerIterator;

window.onload = preparePage;
UrlInput.onblur = getServers;
UrlInput.onkeyup = UrlInput.onchange = caluclateHostUrl;
RefreshButton.onclick = getServers;
RequestTypeSelect.onchange = displayRequestFieldsets;

function emptySelect(sel){
	while(sel.length > 0){
	sel.remove(sel.length-1);
	}
}

function preparePage(){
	UrlInput.value = document.location.hostname;
	displayRequestFieldsets();
	caluclateHostUrl();
	getServers();
}

function caluclateHostUrl(){
	if(!UrlInput.value){
		return;
	}
	if(!UrlInput.value.match(/http*/)){
		HostUrl = "http://" + UrlInput.value;
	} else {
		HostUrl = UrlInput.value;
	}
	HostUrl.trim();
	if(!UrlInput.value.match(/:/)){
		HostName = HostUrl;
		HostUrl += ":7509";
	} else {
		HostName = HostUrl.split(":")[0] + ":" + HostUrl.split(":")[1];
	}
}

function displayRequestFieldsets(){
	if(RequestTypeSelect.options[RequestTypeSelect.options.selectedIndex].value == "GET"){
		FieldSetGET.style.display = "inline";
		FieldSetPOST.style.display = "none";
		FieldSetMSG.style.display = "none";
		FieldSetSVC.style.display = "none";
	} else if(RequestTypeSelect.options[RequestTypeSelect.options.selectedIndex].value == "POST"){
		FieldSetGET.style.display = "none";
		FieldSetPOST.style.display = "inline";
		FieldSetMSG.style.display = "none";
		FieldSetSVC.style.display = "none";
	} else if(RequestTypeSelect.options[RequestTypeSelect.options.selectedIndex].value == "MSG"){
		FieldSetGET.style.display = "none";
		FieldSetPOST.style.display = "none";
		FieldSetMSG.style.display = "inline";
		FieldSetSVC.style.display = "none";
	} else if(RequestTypeSelect.options[RequestTypeSelect.options.selectedIndex].value == "SVCCALL"){
		FieldSetGET.style.display = "none";
		FieldSetPOST.style.display = "none";
		FieldSetMSG.style.display = "none";
		FieldSetSVC.style.display = "inline";
	}
}

function getServers(){
//test servers on url
	if(HostUrl){
		if (window.XMLHttpRequest)
		{// code for IE7+, Firefox, Chrome, Opera, Safari
			var req = new XMLHttpRequest();
		} else {// code for IE6, IE5
			var req = new ActiveXObject("Microsoft.XMLHTTP");
		}
		req.open("GET", HostUrl + "/getEP?path=/servers&format=ksx&requestType=OT_DOMAIN", true);
		req.onreadystatechange = startGetServerIteration;
		try{
			req.send(null);
		}catch(e){
			PlaygroundOutput.innerHTML = "transmit error: "+e;
		};
	}
};

function startGetServerIteration(evt){
	if (!evt){
		var req = this;
	}else{
		req = evt.target;
	}
	if( req.readyState == 4 ){
		if(req.status == 200){
			if(!req.responseText){
				return;
			}
			if (window.DOMParser) {
				parser = new DOMParser();
				xmlResponse = parser.parseFromString(req.responseText,"text/xml");
			} else // Internet Explorer
			{
				xmlResponse = new ActiveXObject("Microsoft.XMLDOM");
				xmlResponse.async = false;
				xmlResponse.loadXML(req.responseText);
			} 
			emptySelect(ServerList);
			var baseGetEp = xmlResponse.getElementsByTagName("getep")[0];
			ServerInstances = baseGetEp.childNodes;
			for (var i = 0; i < ServerInstances.length; i++){
				if(ServerInstances[i].tagName == "DomainEngProps" 
					&& ServerInstances[i].getElementsByTagName("identifier")[0]){
					var servername = ServerInstances[i].getElementsByTagName("identifier")[0].textContent;
					var option = document.createElement("option");
					option.value = servername;
					option.text = servername;
					option.disabled = true;
					ServerList.add(option);
				}
			}
			ServerIterator = 0;
			if(ServerIterator < ServerList.length){
				if (window.XMLHttpRequest)
				{// code for IE7+, Firefox, Chrome, Opera, Safari
					var getServer = new XMLHttpRequest();
				} else {// code for IE6, IE5
					var getServer = new ActiveXObject("Microsoft.XMLHTTP");
				}
				getServer.open("GET", HostUrl + "/getServer?format=ksx&servername=" + ServerList[ServerIterator].text, true);
				getServer.onreadystatechange = GetNextServer;
				try{
					getServer.send(null);
				}catch(e){
					PlaygroundOutput.innerHTML = "transmit error getting server: "+e;
				};
			}
		} else {
			PlaygroundOutput.innerHTML = "error";
		}
	}
}

function GetNextServer(evt){
	if (!evt){
		var req = this;
	}else{
		req = evt.target;
	}
	if( req.readyState == 4 ){
		if(req.status == 200){
			if(!req.responseText){
				return;
			}
			if (window.DOMParser) {
				parser = new DOMParser();
				xmlResponse = parser.parseFromString(req.responseText,"text/xml");
			} else // Internet Explorer
			{
				xmlResponse = new ActiveXObject("Microsoft.XMLDOM");
				xmlResponse.async = false;
				xmlResponse.loadXML(req.responseText);
			} 
			var port = xmlResponse.getElementsByTagName("port")[0].innerHTML;
			if(port){
				ServerList.options[ServerIterator].value = port;
				ServerList.options[ServerIterator].disabled = false;
				ServerIterator++;
			}
			if(ServerIterator < ServerList.length){
				if (window.XMLHttpRequest)
				{// code for IE7+, Firefox, Chrome, Opera, Safari
					var getNextServerReq = new XMLHttpRequest();
				} else {// code for IE6, IE5
					var getNextServerReq = new ActiveXObject("Microsoft.XMLHTTP");
				}
				getNextServerReq.open("GET", HostUrl + "/getServer?format=ksx&servername=" + ServerList[ServerIterator].text, true);
				getNextServerReq.onreadystatechange = GetNextServer;
				try{
					getNextServerReq.send(null);
				}catch(e){
					PlaygroundOutput.innerHTML = "transmit error getting server: "+e;
				};
			}
		} else {
			PlaygroundOutput.innerHTML = "error";
		}
	}
}

Submitbutton.onclick = document.getElementsByTagName("form")[0].onsubmit = function(evt){
	if(HostUrl){
		if(RequestTypeSelect.options[RequestTypeSelect.options.selectedIndex].value == "GET"){
			SubmitGetRequest();
		} else if(RequestTypeSelect.options[RequestTypeSelect.options.selectedIndex].value == "POST"){
			SubmitPostRequest();
		} else if(RequestTypeSelect.options[RequestTypeSelect.options.selectedIndex].value == "MSG"){
			SubmitMessage();
		} else if(RequestTypeSelect.options[RequestTypeSelect.options.selectedIndex].value == "SVCCALL"){
			SubmitServiceCall();
		}
	}
	return false;
};

function SubmitGetRequest(){
	var req = new XMLHttpRequest();
	if(!HostName || !ServerList.options[ServerList.options.selectedIndex] || !ServerList.options[ServerList.options.selectedIndex].value){
		return;
	}
	var requestString = HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + RequestGetURL.value;
	req.open("GET", requestString, true);
	req.timeout = 10000;
	req.onreadystatechange = RequestStatehandler;
	try{
		req.send(null);
	}catch(err){
		PlaygroundOutput.innerHTML = "transmit error: "+err;
		PlaygroundOutput.style.borderColor = "red";
	}
}

function SubmitPostRequest(){
	var req = new XMLHttpRequest();
	if(!HostName || !ServerList.options[ServerList.options.selectedIndex] || !ServerList.options[ServerList.options.selectedIndex].value){
		return;
	}
	var requestString = HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + RequestPostURL.value;
	req.open("POST", requestString, true);
	req.timeout = 10000;
	req.onreadystatechange = RequestStatehandler;
	try{
		req.send(UrlBodyInput);
	}catch(err){
		PlaygroundOutput.innerHTML = "transmit error: "+err;
		PlaygroundOutput.style.borderColor = "red";
	}
}

function SubmitMessage(){
	if(!HostName || !ServerList.options[ServerList.options.selectedIndex] || !ServerList.options[ServerList.options.selectedIndex].value || !MsgRcvAddress.value){
		return;
	}
	var MessageDoc = createBasicMessage(HostName, ServerList.options[ServerList.options.selectedIndex].value, MsgRcvAddress.value)
	var newtext = MessageDoc.createTextNode(MsgBody.value);
	MessageDoc.Body.appendChild(newtext);
	PlaygroundOutput.innerHTML = getMessageString(MessageDoc);
	
	var req = new XMLHttpRequest();
	
	var requestString = HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/msg";
	req.open("POST", requestString, true);
	req.timeout = 10000;
	req.setRequestHeader('Content-Type', 'application/xml');
	req.onreadystatechange = RequestStatehandler;
	try{
		req.send(getMessageString(MessageDoc));
	}catch(err){
		PlaygroundOutput.innerHTML = "transmit error: "+err;
		PlaygroundOutput.style.borderColor = "red";
	}
}

function SubmitServiceCall(){
	if(!HostName || !ServerList.options[ServerList.options.selectedIndex] || !ServerList.options[ServerList.options.selectedIndex].value || !SvcRcvAddress.value || !SvcCallSvc.value || !SvcCallOp.value){
		return;
	}
	var MessageDoc = createBasicMessage(HostName, ServerList.options[ServerList.options.selectedIndex].value, SvcRcvAddress.value)
	MessageDoc.Body.setAttribute("Svc", SvcCallSvc.value); 
	MessageDoc.Body.setAttribute("Op", SvcCallOp.value); 
	var newtext = MessageDoc.createTextNode(SvcCallBody.value);
	MessageDoc.Body.appendChild(newtext);	
	
	PlaygroundOutput.innerHTML = getMessageString(MessageDoc);
	
	var req = new XMLHttpRequest();
	
	var requestString = HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/msg";
	req.open("POST", requestString, true);
	req.timeout = 10000;
	req.setRequestHeader('Content-Type', 'application/xml');
	req.onreadystatechange = RequestStatehandler;
	try{
		req.send(getMessageString(MessageDoc));
	}catch(err){
		PlaygroundOutput.innerHTML = "transmit error: "+err;
		PlaygroundOutput.style.borderColor = "red";
	}
}

function RequestStatehandler(evt){
	if (!evt){
		var req = this;
	}else{
		req = evt.target;
	}
	if( req.readyState == 4 ){
		if(req.status == 0){
			PlaygroundOutput.innerHTML = "The request failed. Timeout?";
		}else{
			PlaygroundOutput.innerHTML = req.responseText;
		}
		
		if(req.status == 200){
			PlaygroundOutput.style.borderColor = "#8080FF";
			Submitbutton.style.backgroundColor = "limegreen";
			Submitbutton.value = "response received";
		}else{
			PlaygroundOutput.style.borderColor = "red";
			Submitbutton.style.backgroundColor = "red";
			Submitbutton.value = "error received";
		}
		var timeoutID = window.setTimeout(function(){
			Submitbutton.value = "send request";
			Submitbutton.style.backgroundColor = "";
		},1500);
	}
}
