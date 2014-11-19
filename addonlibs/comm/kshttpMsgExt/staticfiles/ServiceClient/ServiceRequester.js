var Submitbutton = document.getElementById("idSubmitbutton");
var RefreshButton = document.getElementById("idRefresh");
var UrlInput = document.getElementById("idUrlInput");
var ResponseOutput = document.getElementById("idResponseOutput");
var ServerList = document.getElementById("idServersSelect");
var ServiceFieldSet = document.getElementById("idSVCFieldset");
var NoProviderParagraph = document.getElementById("idNoServiceAvailable");
var ServiceList = document.getElementById("idServiceSelect");
var ServiceTypeText = document.getElementById("idServiceType");
var ServiceDesciptionText = document.getElementById("idServiceDesciption");
var OperationField = document.getElementById("idOperationField");
var OperationList = document.getElementById("idOperationsSelect");
var OperationDocumentation = document.getElementById("idOperationDocumentation");
var OperationOfService = document.getElementById("idOutputServiceName");
var ParametersFieldset = document.getElementById("idParametersFieldset");
var ParamsOfOperation = document.getElementById("idOutputOperationName");
var InParamsTable = document.getElementById("idInParameters");
var OutParamsTable = document.getElementById("idOutParameters");

var HostUrl;
var HostName;
var ServerInstances;
var ServerIterator;
var Services;
var ServiceTypes;
var ServiceIsActives;
var ServicesDescriptions;
var Operations;
var OperationDocumentations;
var inParameters;
var inParamIds;
var inTypes;
var outParameters;
var outParamIds;
var outTypes;


window.onload = preparePage;
UrlInput.onblur = getServers;
UrlInput.onkeyup = UrlInput.onchange = function (){
	caluclateHostUrl();
	Submitbutton.disabled = true;
}
RefreshButton.onclick = getServers;
ServerList.onchange = checkServiceProvider;
ServiceList.onchange = displayServiceData;
OperationList.onchange = displayOperationData;


function emptySelect(sel){
	while(sel.length > 0){
	sel.remove(sel.length-1);
	}
}

function preparePage(){
	UrlInput.value = document.location.hostname;
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
		req.timeout = 10000;
		try{
			req.send(null);
		}catch(e){
			ResponseOutput.innerHTML = "transmit error: "+e;
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
					var servername = ServerInstances[i].getElementsByTagName("identifier")[0].innerHTML;
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
				getServer.timeout = 10000;
				try{
					getServer.send(null);
				}catch(e){
					ResponseOutput.innerHTML = "transmit error getting server: "+e;
				};
			}
		} else {
			ResponseOutput.innerHTML = "error";
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
				getNextServerReq.timeout = 10000;
				try{
					getNextServerReq.send(null);
				}catch(e){
					ResponseOutput.innerHTML = "transmit error getting server: "+e;
				};
			}
		} else {
			ResponseOutput.innerHTML = "error";
		}
	}
}

function checkServiceProvider(){
//test servers on url
	if(HostUrl){
		OperationField.style.display = "none";
		ParametersFieldset.style.display = "none";
		Submitbutton.disabled = true;
		if (window.XMLHttpRequest)
		{// code for IE7+, Firefox, Chrome, Opera, Safari
			var req = new XMLHttpRequest();
		} else {// code for IE6, IE5
			var req = new ActiveXObject("Microsoft.XMLHTTP");
		}
		req.open("GET", HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/getVar?path=/acplt/ov/class.instance&format=ksx", true);
		req.onreadystatechange = handleResponseCheckprovider;
		req.timeout = 10000;
		try{
			req.send(null);
		}catch(e){
			ResponseOutput.innerHTML = "transmit error: "+e;
		};
	}
};

function handleResponseCheckprovider(evt){
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
			var baseGetVar = xmlResponse.getElementsByTagName("stringvec")[0];
			var Instances = baseGetVar.childNodes;
			for (var i = 0; i < Instances.length; i++){
				if(Instances[i].tagName == "string"){
					if(Instances[i].innerHTML.match(/ServiceProvider\/API/)){
						ResponseOutput.innerHTML = "ServiceProvider available!";
						ServiceFieldSet.style.display = "inline";
						NoProviderParagraph.style.display = "none";
						ParametersFieldset.style.display = "none";
						Submitbutton.disabled = true;
						emptySelect(ServiceList);
						getServicesOfProvider();
						return;
					}
				}
			}
				ResponseOutput.innerHTML = "ServiceProvider NOT available!";
				ServiceFieldSet.style.display = "none";
				NoProviderParagraph.style.display = "inline";
				ParametersFieldset.style.display = "none";
				Submitbutton.disabled = true;
		} else {
			ResponseOutput.innerHTML = "error";
		}
	}
}

function getServicesOfProvider(){
	OperationField.style.display = "none";
	ParametersFieldset.style.display = "none";
	Submitbutton.disabled = true;
	var MessageDoc = CreateGenericServiceCall(HostName, ServerList.options[ServerList.options.selectedIndex].value, "Exploration", "Exploration", "getServices");
	if (window.XMLHttpRequest)
	{// code for IE7+, Firefox, Chrome, Opera, Safari
		var req = new XMLHttpRequest();
	} else {// code for IE6, IE5
		var req = new ActiveXObject("Microsoft.XMLHTTP");
	}
	req.open("POST", HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/msg", true);
	req.onreadystatechange = handleResponsegetServices;
	req.timeout = 10000;
	try{
		req.send(getMessageString(MessageDoc));
	}catch(e){
		ResponseOutput.innerHTML = "transmit error: "+e;
	};
}

function handleResponsegetServices(evt){
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
			Services = new Array();
			ServiceTypes = new Array();
			ServiceIsActives = new Array();
			ServicesDescriptions = new Array();
			emptySelect(ServiceList);
			try{
				var Body = xmlResponse.getElementsByTagName("bdy")[0];
				var servicesString = Body.getElementsByTagName("val")[0].innerHTML;
				var serviceTypesString = Body.getElementsByTagName("val")[1].innerHTML;
				var isActiveString = Body.getElementsByTagName("val")[2].innerHTML;
				var DefinitionsString = Body.getElementsByTagName("val")[3].innerHTML;
				var servicesList = servicesString.split(";");
				var serviceTypesList = serviceTypesString.split(";");
				var isActiveList = isActiveString.split(";");
				var DefinitionsList = DefinitionsString.split(";");
				
				for(i in servicesList){
					Services.push(servicesList[i]);
					ServiceTypes.push(serviceTypesList[i]);
					ServiceIsActives.push(isActiveList[i]);
					ServicesDescriptions.push(DefinitionsList[i]);
					var option = document.createElement("option");
					option.value = servicesList[i];
					option.text = servicesList[i];
					option.disabled = !isActiveList[i];
					ServiceList.add(option);
				}
			} catch(err){
				ResponseOutput.innerHTML = "error getting registered services: "+err;
			}			
		} else {
			ResponseOutput.innerHTML = "error";
		}
	}
}

function displayServiceData(){
	OperationField.style.display = "none";
	ParametersFieldset.style.display = "none";
	Submitbutton.disabled = true;
	ServiceTypeText.innerHTML = ServiceTypes[ServiceList.options.selectedIndex];
	ServiceDesciptionText.innerHTML = ServicesDescriptions[ServiceList.options.selectedIndex];
	getOperationsInfo();
	OperationOfService.innerHTML = Services[ServiceList.options.selectedIndex];
}

function getOperationsInfo(){
	var MessageDoc = CreateGenericServiceCall(HostName, ServerList.options[ServerList.options.selectedIndex].value, "Exploration", "Exploration", "getOperations");
	var sd = AddStructData(MessageDoc, MessageDoc.Body, "getoperationsRequest")
	AddValue(MessageDoc, sd, "service", ServiceList.options[ServiceList.options.selectedIndex].value, null)
	if (window.XMLHttpRequest)
	{// code for IE7+, Firefox, Chrome, Opera, Safari
		var req = new XMLHttpRequest();
	} else {// code for IE6, IE5
		var req = new ActiveXObject("Microsoft.XMLHTTP");
	}
	req.open("POST", HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/msg", true);
	req.onreadystatechange = handleResponseGetOperations;
	req.timeout = 10000;
	try{
		req.send(getMessageString(MessageDoc));
	}catch(e){
		ResponseOutput.innerHTML = "transmit error: "+e;
	};
}

function handleResponseGetOperations(evt){
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
			Operations = new Array();
			OperationDocumentations = new Array();
			emptySelect(OperationList);
			try{
				var Body = xmlResponse.getElementsByTagName("bdy")[0];
				var operationsString = Body.getElementsByTagName("val")[0].innerHTML;
				var operationDocumentationsString = Body.getElementsByTagName("val")[1].innerHTML;
				var operationsList = operationsString.split(";");
				var documentationsList = operationDocumentationsString.split(";");
				
				for(i in operationsList){
					Operations.push(operationsList[i]);
					OperationDocumentations.push(documentationsList[i]);
					var option = document.createElement("option");
					option.value = operationsList[i];
					option.text = operationsList[i];
					OperationList.add(option);
				}
			} catch(err){
				ResponseOutput.innerHTML = "error getting operations: "+err;
			}			
		} else {
			ResponseOutput.innerHTML = "error";
		}
	}
}

function displayOperationData(){
	OperationField.style.display = "inline";
	ParametersFieldset.style.display = "none";
	Submitbutton.disabled = false;
	OperationDocumentation.innerHTML = OperationDocumentations[OperationList.options.selectedIndex];
	ParamsOfOperation.innerHTML = Operations[OperationList.options.selectedIndex];
	getParametersInfo();
}

function getParametersInfo(){
	var MessageDoc = CreateGenericServiceCall(HostName, ServerList.options[ServerList.options.selectedIndex].value, "Exploration", "Exploration", "getParameters");
	var sd = AddStructData(MessageDoc, MessageDoc.Body, "getParametersRequest")
	AddValue(MessageDoc, sd, "service", ServiceList.options[ServiceList.options.selectedIndex].value, null)
	AddValue(MessageDoc, sd, "operation", OperationList.options[OperationList.options.selectedIndex].value, null)
	if (window.XMLHttpRequest)
	{// code for IE7+, Firefox, Chrome, Opera, Safari
		var req = new XMLHttpRequest();
	} else {// code for IE6, IE5
		var req = new ActiveXObject("Microsoft.XMLHTTP");
	}
	req.open("POST", HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/msg", true);
	req.onreadystatechange = handleResponseGetParameters;
	req.timeout = 10000;
	try{
		req.send(getMessageString(MessageDoc));
	}catch(e){
		ResponseOutput.innerHTML = "transmit error: "+e;
	};
}

function handleResponseGetParameters(evt){
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
			inParameters = new Array();
			inParamIds = new Array();
			inTypes = new Array();
			outParameters = new Array();
			outParamIds = new Array();
			outTypes = new Array();
			try{
				var Body = xmlResponse.getElementsByTagName("bdy")[0];
				var inParametersString = Body.getElementsByTagName("val")[0].innerHTML;
				var inTypesString = Body.getElementsByTagName("val")[1].innerHTML;
				var outParametersString = Body.getElementsByTagName("val")[2].innerHTML;
				var outTypesString = Body.getElementsByTagName("val")[3].innerHTML;
				var inParametersList = inParametersString.split(";");
				var inTypesList = inTypesString.split(";");
				var outParametersList = outParametersString.split(";");
				var outTypesList = outTypesString.split(";");
				while (InParamsTable.firstChild) {
					InParamsTable.removeChild(InParamsTable.firstChild);
				}
				while (OutParamsTable.firstChild) {
					OutParamsTable.removeChild(OutParamsTable.firstChild);
				}
				if(inParametersString){
					for(i in inParametersList){
						inParameters.push(inParametersList[i]);
						inTypes.push(inTypesList[i]);
						var row = document.createElement("tr");
						InParamsTable.appendChild(row);
						var descr = document.createElement("td");
						descr.align = "left";
						descr.width = "400px";
						descr.innerHTML = inParameters[i] + " (" + inTypes[i] + ")";
						row.appendChild(descr);
						var inputCell = document.createElement("td");
						inputCell.align = "left"
						inputCell.width = "400px";
						row.appendChild(inputCell);
						inParamIds.push(document.createElement("input"));
						inParamIds[i].style.width = "350px";
						inputCell.appendChild(inParamIds[i]);
					}
				}
				if(outParametersString){
					for(i in outParametersList){
						outParameters.push(outParametersList[i]);
						outTypes.push(outTypesList[i]);
						var row = document.createElement("tr");
						OutParamsTable.appendChild(row);
						var descr = document.createElement("td");
						descr.align = "left";
						descr.width = "400px";
						descr.innerHTML = outParameters[i] + " (" + outTypes[i] + ")";
						row.appendChild(descr);
						var inputCell = document.createElement("td");
						inputCell.align = "left"
						inputCell.width = "400px";
						row.appendChild(inputCell);
						outParamIds.push(document.createElement("input"));
						outParamIds[i].style.width = "350px";
						outParamIds[i].readOnly = true;
						inputCell.appendChild(outParamIds[i]);
					}
				}
				ParametersFieldset.style.display = "inline";
				Submitbutton.disabled = false;
			} catch(err){
				ResponseOutput.innerHTML = "error getting parameters: "+err;
			}			
		} else {
			ResponseOutput.innerHTML = "error";
		}
	}
}


Submitbutton.onclick = document.getElementsByTagName("form")[0].onsubmit = function(evt){
	if(!HostName || !ServerList.options[ServerList.options.selectedIndex].value || !ServiceList.options[ServiceList.options.selectedIndex].value || !OperationList.options[OperationList.options.selectedIndex].value){
		return;
	}
	var MessageDoc = CreateGenericServiceCall(HostName, ServerList.options[ServerList.options.selectedIndex].value, ServiceList.options[ServiceList.options.selectedIndex].value,
	ServiceList.options[ServiceList.options.selectedIndex].value, OperationList.options[OperationList.options.selectedIndex].value);
	var sd = AddStructData(MessageDoc, MessageDoc.Body, "ServiceRequest")
	for(i in inParamIds){
		AddValue(MessageDoc, sd, inParameters[i], inParamIds[i].value, null)
	}
	if (window.XMLHttpRequest)
	{// code for IE7+, Firefox, Chrome, Opera, Safari
		var req = new XMLHttpRequest();
	} else {// code for IE6, IE5
		var req = new ActiveXObject("Microsoft.XMLHTTP");
	}
	req.open("POST", HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/msg", true);
	req.onreadystatechange = RequestStatehandler;
	req.timeout = 10000;
	try{
		req.send(getMessageString(MessageDoc));
	}catch(e){
		ResponseOutput.innerHTML = "transmit error: "+e;
	};
	return false;
};

function RequestStatehandler(evt){
	if (!evt){
		var req = this;
	}else{
		req = evt.target;
	}
	if( req.readyState == 4 ){
		if(req.status == 0){
			ResponseOutput.innerHTML = "The request failed. Timeout?";
		}
		if(req.status == 200){
			if (window.DOMParser) {
				parser = new DOMParser();
				xmlResponse = parser.parseFromString(req.responseText,"text/xml");
			} else // Internet Explorer
			{
				xmlResponse = new ActiveXObject("Microsoft.XMLDOM");
				xmlResponse.async = false;
				xmlResponse.loadXML(req.responseText);
			} 
			try{
				var Body = xmlResponse.getElementsByTagName("bdy")[0];
				var responseVars = Body.getElementsByTagName("val");
				for(i in outParamIds){
					outParamIds[i].value = responseVars[i].innerHTML;
				}
			} catch(err){
				ResponseOutput.innerHTML = "error parsing service reply: "+err;
			}		


			ResponseOutput.style.borderColor = "#8080FF";
			Submitbutton.style.backgroundColor = "limegreen";
			Submitbutton.value = "response received";
		}else{
			ResponseOutput.style.borderColor = "red";
			Submitbutton.style.backgroundColor = "red";
			Submitbutton.value = "error received";
		}
		var timeoutID = window.setTimeout(function(){
			Submitbutton.value = "send request";
			Submitbutton.style.backgroundColor = "";
		},1500);
	}
}
