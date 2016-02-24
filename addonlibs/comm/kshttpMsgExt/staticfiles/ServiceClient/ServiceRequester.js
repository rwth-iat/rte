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


var PCFiledset = document.getElementById("idPCFieldset");
var noPCParagraph = document.getElementById("idNoPCUsAvailable");
var PCUSelect = document.getElementById("idPCUSelect");
var PCUOperationSelect = document.getElementById("idPCUOperationSelect");
var PCUParameter = document.getElementById("idPCUParameter");
var PCUController = document.getElementById("idPCUController");
var PCUCurrOcc = document.getElementById("idPCUCurrOcc");
var PCUCurrWoSt = document.getElementById("idPCUWoSt");
var PCUCurrError = document.getElementById("idPCUErrSt");
var PCUCurrFM = document.getElementById("idPCUFunctionMode");


var HostUrl;
var HostName;
var requestSystemService = false;
var requestPCService = false;
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
var PCInboxClassPath;


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

PCUSelect.onchange = function(){
	getDataFromPfPCU();
	checkPCUoperationsControlChart();
	PCUCurrFM.disabled = true;
	PCUCurrFM.value = "N/A";
	PCUCurrError.disabled = true;
	PCUCurrError.value = "N/A";
	PCUCurrOcc.disabled = true;
	PCUCurrOcc.value = "N/A";
	PCUCurrWoSt.disabled = true;
	PCUCurrWoSt.value = "N/A";}
	
PCUOperationSelect.onchange = function(){
	requestSystemService = false;
	requestPCService = true;
	Submitbutton.disabled = false;};

function emptySelect(sel){
	while(sel.firstChild){
	sel.removeChild(sel.firstChild);
	}
}

function preparePage(){
	UrlInput.value = document.location.hostname;
	caluclateHostUrl();
	getServers();
	PCUCurrFM.disabled = true;
	PCUCurrFM.value = "N/A";
	PCUCurrError.disabled = true;
	PCUCurrError.value = "N/A";
	PCUCurrOcc.disabled = true;
	PCUCurrOcc.value = "N/A";
	PCUCurrWoSt.disabled = true;
	PCUCurrWoSt.value = "N/A";
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
			var port = xmlResponse.getElementsByTagName("port")[0].textContent;
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
		OperationField.style.maxHeight = "12px";
		ParametersFieldset.style.maxHeight = "12px";
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
			var foundServiceProvider = false;
			var foundPCInbox = false;
			ResponseOutput.innerHTML = "";
			for (var i = 0; i < Instances.length; i++){
				if(Instances[i].tagName == "string"){
					if(!foundServiceProvider && Instances[i].textContent.match(/ServiceProvider\/API/)){
						ResponseOutput.innerHTML += "ServiceProvider available!\n";
						ServiceFieldSet.style.maxHeight = "600px";
						NoProviderParagraph.style.display="none";
						ParametersFieldset.style.maxHeight = "12px";
						Submitbutton.disabled = true;
						emptySelect(ServiceList);
						getServicesOfProvider();
						foundServiceProvider = true;
						if(foundServiceProvider && foundPCInbox){
							return;
						}
						continue;
					}
					if(!foundPCInbox && Instances[i].textContent.match(/PCMsgParser\/PCInbox/)){
						PCInboxClassPath = Instances[i].textContent;
						ResponseOutput.innerHTML += "PCInboxes available!\n";
						PCFiledset.style.maxHeight = "600px";
						noPCParagraph.style.display="none";
						Submitbutton.disabled = true;
						emptySelect(ServiceList);
						getPCInboxes();
						foundPCInbox = true;
						if(foundServiceProvider && foundPCInbox){
							return;
						}
						continue;
					}
				}
			}
				if(!foundServiceProvider){
					ResponseOutput.innerHTML += "ServiceProvider NOT available!\n";
					ServiceFieldSet.style.maxHeight = "12px";
					NoProviderParagraph.style.display="inline";
					ParametersFieldset.style.maxHeight = "12px";
					Submitbutton.disabled = true;
				}
				if(!foundPCInbox){
					ResponseOutput.innerHTML += "NO PCInboxes available!\n";
					PCFiledset.style.maxHeight = "12px";
					noPCParagraph.style.display="inline";
					Submitbutton.disabled = true;
				}
		} else {
			ResponseOutput.innerHTML = "error";
		}
	}
}

function getServicesOfProvider(){
	OperationField.style.maxHeight = "12px";
	ParametersFieldset.style.maxHeight = "12px";
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
				var servicesString = Body.getElementsByTagName("val")[0].textContent;
				var serviceTypesString = Body.getElementsByTagName("val")[1].textContent;
				var isActiveString = Body.getElementsByTagName("val")[2].textContent;
				var DefinitionsString = Body.getElementsByTagName("val")[3].textContent;
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
	OperationField.style.maxHeight = "12px";
	ParametersFieldset.style.maxHeight = "12px";
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
				var operationsString = Body.getElementsByTagName("val")[0].textContent;
				var operationDocumentationsString = Body.getElementsByTagName("val")[1].textContent;
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
	OperationField.style.maxHeight = "600px";
	ParametersFieldset.style.maxHeight = "12px";
	requestSystemService = true;
	requestPCService = false;
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
				var inParametersString = Body.getElementsByTagName("val")[0].textContent;
				var inTypesString = Body.getElementsByTagName("val")[1].textContent;
				var outParametersString = Body.getElementsByTagName("val")[2].textContent;
				var outTypesString = Body.getElementsByTagName("val")[3].textContent;
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
				ParametersFieldset.style.maxHeight = "60000px";
				requestSystemService = true;
				requestPCService = false;
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
	if(!HostName || !ServerList.options[ServerList.options.selectedIndex].value){
		return false;
	}
	if(requestSystemService){
		if(!ServiceList.options[ServiceList.options.selectedIndex].value || !OperationList.options[OperationList.options.selectedIndex].value){
			return false;
		}
		var MessageDoc = CreateGenericServiceCall(HostName, ServerList.options[ServerList.options.selectedIndex].value, ServiceList.options[ServiceList.options.selectedIndex].value,
		ServiceList.options[ServiceList.options.selectedIndex].value, OperationList.options[OperationList.options.selectedIndex].value);
		var sd = AddStructData(MessageDoc, MessageDoc.Body, "ServiceRequest")
		for(i in inParamIds){
			AddValue(MessageDoc, sd, inParameters[i], inParamIds[i].value, null);
		}
	} else if(requestPCService){
		if(!PCUSelect.options[PCUSelect.options.selectedIndex].value || !PCUOperationSelect.options[PCUOperationSelect.options.selectedIndex].value){
			return false;
		}
		var valueNumber = parseFloat(PCUParameter.value);
		if(isNaN(valueNumber)){
			alert("Only numeric values are allowed as parameters!");
			PCUParameter.focus();
			return false;
		}
		var MessageDoc = CreateGenericServiceCall(HostName, ServerList.options[ServerList.options.selectedIndex].value, PCUSelect.options[PCUSelect.options.selectedIndex].value, "ProcessControl", PCUOperationSelect.options[PCUOperationSelect.options.selectedIndex].value);
		AddValue(MessageDoc, MessageDoc.Body, "cmdr", PCUController.value, null);
		AddValue(MessageDoc, MessageDoc.Body, "Value", valueNumber, null);
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
			Submitbutton.style.backgroundColor = "yellow";
			Submitbutton.value = "Awaiting Response";
			Submitbutton.disabled = true;
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
			if(requestSystemService){
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
						outParamIds[i].value = responseVars[i].textContent;
					}
				} catch(err){
					ResponseOutput.innerHTML = "error parsing service reply: "+err;
				}
			} else if(requestPCService){
				ResponseOutput.innerHTML = req.responseText;
			}
			ResponseOutput.style.borderColor = "#8080FF";
			Submitbutton.style.backgroundColor = "limegreen";
			Submitbutton.value = "response received";
			Submitbutton.disabled = false;
		}else{
			ResponseOutput.style.borderColor = "red";
			Submitbutton.style.backgroundColor = "red";
			Submitbutton.value = "error received";
			Submitbutton.disabled = false;
		}
		var timeoutID = window.setTimeout(function(){
			Submitbutton.value = "send request";
			Submitbutton.style.backgroundColor = "";
		},1500);
	}
}

function getPCInboxes(){
//test servers on url
	if(HostUrl){
		if (window.XMLHttpRequest)
		{// code for IE7+, Firefox, Chrome, Opera, Safari
			var req = new XMLHttpRequest();
		} else {// code for IE6, IE5
			var req = new ActiveXObject("Microsoft.XMLHTTP");
		}
		req.open("GET", HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/getVar?path=" + PCInboxClassPath + ".instance&format=ksx", true);
		req.onreadystatechange = handleResponsegetPCInboxes;
		req.timeout = 10000;
		try{
			req.send(null);
		}catch(e){
			ResponseOutput.innerHTML = "transmit error: "+e;
		};
	}
};

function handleResponsegetPCInboxes(evt){
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
			emptySelect(PCUSelect);
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
			for (i in Instances){
				if(Instances[i].tagName == "string"){
					var splitPath = Instances[i].textContent.split("/");
					var showPath = "";
					for(var j = 1; j < splitPath.length - 1; j++){
						showPath += "/" + splitPath[j];
					}
					var option = document.createElement("option");
					option.value = showPath;
					option.text = showPath;
					PCUSelect.add(option);
				}
			}			
		}
	}
}

function checkPCUoperationsControlChart(){
//test servers on url
	Submitbutton.disabled = true;
	if(HostUrl){
		if (window.XMLHttpRequest)
		{// code for IE7+, Firefox, Chrome, Opera, Safari
			var req = new XMLHttpRequest();
		} else {// code for IE6, IE5
			var req = new ActiveXObject("Microsoft.XMLHTTP");
		}
		req.open("GET", HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/getVar?path[0]=" + PCUSelect.options[PCUSelect.options.selectedIndex].value + ".ORDERLIST&format=ksx", true);
		req.onreadystatechange = handlecheckPCUoperationsControlChart;
		req.timeout = 10000;
		try{
			req.send(null);
		}catch(e){
			ResponseOutput.innerHTML = "transmit error: "+e;
		};
	}
};

function handlecheckPCUoperationsControlChart(evt){
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
			// cut off everything before "<"
			responseText = req.responseText.substring(req.responseText.indexOf("<"));
			
			emptySelect(PCUOperationSelect);
			if (window.DOMParser) {
				parser = new DOMParser();
				xmlResponse = parser.parseFromString(responseText,"text/xml");
			} else // Internet Explorer
			{
				xmlResponse = new ActiveXObject("Microsoft.XMLDOM");
				xmlResponse.async = false;
				xmlResponse.loadXML(responseText);
			} 
			var baseGetVar = xmlResponse.getElementsByTagName("stringvec")[0];
			if(baseGetVar){
				var OperationStrings = baseGetVar.childNodes;
				for (i in OperationStrings){
					if(OperationStrings[i].tagName == "string"){
						var option = document.createElement("option");
						option.value = OperationStrings[i].textContent;
						option.text = OperationStrings[i].textContent;
						PCUOperationSelect.add(option);
					}
				}
			}
		} else if (req.status == 404){
			checkPCUoperationsFC();
		}
	}
}

function checkPCUoperationsFC(){
//test servers on url
	Submitbutton.disabled = true;
	if(HostUrl){
		if (window.XMLHttpRequest)
		{// code for IE7+, Firefox, Chrome, Opera, Safari
			var req = new XMLHttpRequest();
		} else {// code for IE6, IE5
			var req = new ActiveXObject("Microsoft.XMLHTTP");
		}
		req.open("GET", HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/getVar?path[0]=" + PCUSelect.options[PCUSelect.options.selectedIndex].value + "/possibleCommands.value&format=ksx", true);
		req.onreadystatechange = handlecheckPCUoperationsFC;
		req.timeout = 10000;
		try{
			req.send(null);
		}catch(e){
			ResponseOutput.innerHTML = "transmit error: "+e;
		};
	}
};

function handlecheckPCUoperationsFC(evt){
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
			// cut off everything before "<"
			responseText = req.responseText.substring(req.responseText.indexOf("<"));
			
			emptySelect(PCUOperationSelect);
			if (window.DOMParser) {
				parser = new DOMParser();
				xmlResponse = parser.parseFromString(responseText,"text/xml");
			} else // Internet Explorer
			{
				xmlResponse = new ActiveXObject("Microsoft.XMLDOM");
				xmlResponse.async = false;
				xmlResponse.loadXML(responseText);
			} 
			var baseGetVar = xmlResponse.getElementsByTagName("stringvec")[0];
			if(baseGetVar){
				var OperationStrings = baseGetVar.childNodes;
				for (i in OperationStrings){
					if(OperationStrings[i].tagName == "string"){
						var option = document.createElement("option");
						option.value = OperationStrings[i].textContent;
						option.text = OperationStrings[i].textContent;
						PCUOperationSelect.add(option);
					}
				}
				var option = document.createElement("option");
				option.value = "START";
				option.text = "START";
				PCUOperationSelect.add(option);
				option = document.createElement("option");
				option.value = "STOP";
				option.text = "STOP";
				PCUOperationSelect.add(option);
				option = document.createElement("option");
				option.value = "OCCUPY";
				option.text = "OCCUPY";
				PCUOperationSelect.add(option);
				option = document.createElement("option");
				option.value = "FREE";
				option.text = "FREE";
				PCUOperationSelect.add(option);
			}
		} else if (req.status == 404) {
			checkPCUoperationsPC();
		}
	}
}

function checkPCUoperationsPC(){
//test servers on url
	Submitbutton.disabled = true;
	if(HostUrl){
		if (window.XMLHttpRequest)
		{// code for IE7+, Firefox, Chrome, Opera, Safari
			var req = new XMLHttpRequest();
		} else {// code for IE6, IE5
			var req = new ActiveXObject("Microsoft.XMLHTTP");
		}
		req.open("GET", HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/getVar?path[0]=" + PCUSelect.options[PCUSelect.options.selectedIndex].value + ".PossibleCmds&format=ksx", true);
		req.onreadystatechange = handlecheckPCUoperationsPC;
		req.timeout = 10000;
		try{
			req.send(null);
		}catch(e){
			ResponseOutput.innerHTML = "transmit error: "+e;
		};
	}
};

function handlecheckPCUoperationsPC(evt){
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
			// cut off everything before "<"
			responseText = req.responseText.substring(req.responseText.indexOf("<"));
			
			emptySelect(PCUOperationSelect);
			if (window.DOMParser) {
				parser = new DOMParser();
				xmlResponse = parser.parseFromString(responseText,"text/xml");
			} else // Internet Explorer
			{
				xmlResponse = new ActiveXObject("Microsoft.XMLDOM");
				xmlResponse.async = false;
				xmlResponse.loadXML(responseText);
			} 
			var baseGetVar = xmlResponse.getElementsByTagName("stringvec")[0];
			if(baseGetVar){
				var OperationStrings = baseGetVar.childNodes;
				for (i in OperationStrings){
					if(OperationStrings[i].tagName == "string"){
						var option = document.createElement("option");
						option.value = OperationStrings[i].textContent;
						option.text = OperationStrings[i].textContent;
						PCUOperationSelect.add(option);
					}
				}
				var option = document.createElement("option");
				option.value = "START";
				option.text = "START";
				PCUOperationSelect.add(option);
				option = document.createElement("option");
				option.value = "STOP";
				option.text = "STOP";
				PCUOperationSelect.add(option);
				option = document.createElement("option");
				option.value = "OCCUPY";
				option.text = "OCCUPY";
				PCUOperationSelect.add(option);
				option = document.createElement("option");
				option.value = "FREE";
				option.text = "FREE";
				PCUOperationSelect.add(option);
			}
		}
	}
}

function getDataFromPfPCU(){
	if(!HostName || !ServerList.options[ServerList.options.selectedIndex].value || !PCUSelect.options[PCUSelect.options.selectedIndex].value){
		return;
	}
	if (window.XMLHttpRequest)
		{// code for IE7+, Firefox, Chrome, Opera, Safari
			var req = new XMLHttpRequest();
		} else {// code for IE6, IE5
			var req = new ActiveXObject("Microsoft.XMLHTTP");
		}
		req.open("GET", HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/getVar?path[0]=" + PCUSelect.options[PCUSelect.options.				selectedIndex].value + ".Occupier&path[1]=.WorkingState&path[2]=.FunctionMode&path[3]=.ErrorString&format=ksx", true);
		req.onreadystatechange = handleResponseGetDataFromPfPCU;
		req.timeout = 10000;
		try{
			req.send(null);
		}catch(e){
			ResponseOutput.innerHTML = "transmit error: "+e;
		};
}

function handleResponseGetDataFromPfPCU(evt){
	if (!evt){
		var req = this;
	}else{
		req = evt.target;
	}
	if( req.readyState == 4 ){
		if(req.status == 0){
			ResponseOutput.innerHTML = "The request failed. Timeout?";
			var timeoutID = window.setTimeout(function(){
					getDataFromPfPCU();
				},2000);
		}
		// we will get a 404 since we check pflib PCUs and fc PCUs at the same time. Only one will work
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
			var values = xmlResponse.getElementsByTagName("value");
			if(values.length >= 4){
				var Children = values[0].childNodes;
				for (i in Children){
					if(Children[i].tagName == "string"){
						PCUCurrOcc.value = Children[i].textContent;
						PCUCurrOcc.disabled = false;
						break;
					}
				}
				Children = values[1].childNodes;
				for (i in Children){
					if(Children[i].tagName == "uint"){
						PCUCurrWoSt.value = Children[i].textContent;
						PCUCurrWoSt.disabled = false;
						break;
					}
				}
				Children = values[2].childNodes;
				for (i in Children){
					if(Children[i].tagName == "string"){
						PCUCurrFM.value = Children[i].textContent;
						PCUCurrFM.disabled = false;
						break;
					}
				}
				Children = values[3].childNodes;
				for (i in Children){
					if(Children[i].tagName == "stringvec"){
						var Strings = Children[i].childNodes;
						var first = true;
						for(j in Strings){
							if(Strings[j].tagName == "string"){
								if(first){
									PCUCurrError.value = Strings[j].textContent;
									first = false;
								} else {
									PCUCurrError.value += " | " + Strings[j].textContent;
								}
							}
						}
						PCUCurrError.disabled = false;
						break;
					}
				}
				var timeoutID = window.setTimeout(function(){
					getDataFromPfPCU();
				},1000);
			}
		} else if(req.status == 404){
			var timeoutID = window.setTimeout(function(){
				getDataFromFcPCU();
			},1000);
			return;
		}
	}
}	


function getDataFromFcPCU(){
	if(!HostName || !ServerList.options[ServerList.options.selectedIndex].value || !PCUSelect.options[PCUSelect.options.selectedIndex].value){
		return;
	}
	if (window.XMLHttpRequest)
		{// code for IE7+, Firefox, Chrome, Opera, Safari
			var req = new XMLHttpRequest();
		} else {// code for IE6, IE5
			var req = new ActiveXObject("Microsoft.XMLHTTP");
		}
		req.open("GET", HostName + ":" + ServerList.options[ServerList.options.selectedIndex].value + "/getVar?path[0]=" + PCUSelect.options[PCUSelect.options.selectedIndex].value + "/Occupier.value&path[1]=../workingState.value&format=ksx", 				true);
		req.onreadystatechange = handleResponseGetDataFromFcPCU;
		req.timeout = 10000;
		try{
			req.send(null);
		}catch(e){
			ResponseOutput.innerHTML = "transmit error: "+e;
		};
}


function handleResponseGetDataFromFcPCU(evt){
	if (!evt){
		var req = this;
	}else{
		req = evt.target;
	}
	if( req.readyState == 4 ){
		if(req.status == 0){
			ResponseOutput.innerHTML = "The request failed. Timeout?";
			var timeoutID = window.setTimeout(function(){
					getDataFromFcPCU();
				},2000);
		}
		// we will get a 404 since we check pflib PCUs and fc PCUs at the same time. Only one will work
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
			var values = xmlResponse.getElementsByTagName("value");
			if(values.length >= 2){
				var Children = values[0].childNodes;
				for (i in Children){
					if(Children[i].tagName == "string"){
						PCUCurrOcc.value = Children[i].textContent;
						PCUCurrOcc.disabled = false;
						break;
					}
				}
				Children = values[1].childNodes;
				for (i in Children){
					if(Children[i].tagName == "string"){
						PCUCurrWoSt.value = Children[i].textContent;
						PCUCurrWoSt.disabled = false;
						break;
					}
				}
				PCUCurrFM.disabled = true;
				PCUCurrFM.value = "N/A";
				PCUCurrError.disabled = true;
				PCUCurrError.value = "N/A";
				}
				var timeoutID = window.setTimeout(function(){
					getDataFromFcPCU();
				},1000);
		} else if(req.status == 404){
			var timeoutID = window.setTimeout(function(){
				getDataFromPfPCU();
			},1000);
			return;
		}
	}
}	