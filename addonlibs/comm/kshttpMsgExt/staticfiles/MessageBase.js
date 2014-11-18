function createBasicMessage(host, port, receiverAddress){
	if (window.DOMParser) {
		parser = new DOMParser();
		var MessageDoc = parser.parseFromString("<env></env>","text/xml");
	} else // Internet Explorer
	{
		var MessageDoc = new ActiveXObject("Microsoft.XMLDOM");
		MessageDoc.async = false;
		MessageDoc.loadXML("<env></env>");
	} 
	var MessageContainer = MessageDoc.getElementsByTagName("env")[0];
	var msg = MessageDoc.createElement("msg");
	MessageContainer.appendChild(msg);
	var hdr = MessageDoc.createElement("hdr");
	msg.appendChild(hdr);
	
	var newel = MessageDoc.createElement("rcvSysAdr");
	var newtext = MessageDoc.createTextNode(host + ":" + port);
	newel.appendChild(newtext);
	hdr.appendChild(newel);
	
	newel = MessageDoc.createElement("rcvLocAdr");
	newtext = MessageDoc.createTextNode(receiverAddress);
	newel.appendChild(newtext);
	hdr.appendChild(newel);
	
	newel = MessageDoc.createElement("msgId");
	newtext = MessageDoc.createTextNode(Math.floor((Math.random() * 2000000000)));
	newel.appendChild(newtext);
	hdr.appendChild(newel);
	
	newel = MessageDoc.createElement("sndSysAdr");
	newtext = MessageDoc.createTextNode("clientHost:WebClient");
	newel.appendChild(newtext);
	hdr.appendChild(newel);
	
	newel = MessageDoc.createElement("sndLocAdr");
	newtext = MessageDoc.createTextNode("WebClient");
	newel.appendChild(newtext);
	hdr.appendChild(newel);
	
	var Body = MessageDoc.createElement("bdy");
	msg.appendChild(Body);
	MessageDoc.Body = Body;
	MessageDoc.Header = hdr;
	MessageDoc.Container = MessageContainer;
	return MessageDoc;
}

function getMessageString(MessageDoc){
	return MessageDoc.Container.innerHTML;
}