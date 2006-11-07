/*********************************
	Global Variables
*********************************/

var HMI = new HMI();

/*********************************
	Add SVGComponent
*********************************/

function fncOnClickAddVarValue() {
	var d1 = new Date();
	
	//	init Communication
	//
	HMI.KSClient.init(document.getElementById('idHostAndServer').value, document.getElementById('idTCLKSGateway').value) != null
   if (HMI.KSClient.TCLKSHandle != null)
   {
	   HMI.addComponent('idNewPlayground', document.getElementById('idPath0').value);
	   HMI.addComponent('idNewPlayground', document.getElementById('idPath1').value);
	   
	   HMI.initGestures();
	};
	
	var d2 = new Date();	
   document.getElementById('idCalcTime').value = d2.getTime() - d1.getTime();
   
   delete d1;
 	delete d2;
};

/*********************************
	Replace Component
*********************************/

function fncOnClickRefresh() {	
	var d1 = new Date();
	
   if (HMI.KSClient.TCLKSHandle != null)
   {
		HMI.replaceComponent('idNewPlayground', document.getElementById('idPath0').value);
		HMI.replaceComponent('idNewPlayground', document.getElementById('idPath1').value);
		HMI.initGestures();
   } else {
   	fncOnClickAddVarValue();
	};
		
   var d2 = new Date();
   document.getElementById('idCalcTime').value = d2.getTime() - d1.getTime();
 
 	delete d1;
 	delete d2;
};

/*********************************
	Replace Component automatically
*********************************/

function fncOnClickAutoRefresh() {	
	var d1 = new Date();
	
   if (HMI.KSClient.TCLKSHandle != null)
   {
		HMI.replaceComponent('idNewPlayground', document.getElementById('idPath0').value);
		HMI.replaceComponent('idNewPlayground', document.getElementById('idPath1').value);
		HMI.initGestures();
	 	   
	   HMI.RefreshTimeoutID = setTimeout("fncOnClickAutoRefresh()", document.getElementById('idRefreshTime').value);
   } else {
   	fncOnClickAddVarValue();
   	fncOnClickAutoRefresh();
	};
		
   var d2 = new Date();
   document.getElementById('idCalcTime').value = d2.getTime() - d1.getTime();
 
 	delete d1;
 	delete d2;
};