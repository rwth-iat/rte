Funktion der ksserv:
-	stellt TCP-Port zur verfügung --> verarbeitet eingehende KS-Befehle und eventuell manager-Befehle
-	legt ein ksmanagercom Object an, welches die gesamte Kommunikation mit dem Manger abwickelt (zyklisches register, unregister)
-	übernimmt eventuell manager-Funktion
-	stellt eventuell UDP-Port zur Verfügung --> verarbeitet eingehende manager-Befehle


externe Variablen:

-	OWNPORT:	TCP-Port unter dem der Server beim Manager registriert wird --> wird im tcpconnection Object unter port gespeichert und im managercom Object unter sport
			Wenn nicht gesetzt --> standardmäßig 7509
-	OWNNAME:	Name mit dem der Server beim Manager registriert wird (es wird ein ov_ vorangestellt) --> wird im managercom Object unter sname gespeichert
			Wenn nicht gesetz -->	wenn OWNPORT != 7509 	--> ov_vendortree_getservername
						sonst			--> MANAGER
-	MNGLIBPORT:	Port unter dem die manager-Komponente per UDP erreichbar ist.
			Wenn nicht gesetzt --> standardmäßig 7509

	
Verhalten:
-	keine externen Variablen gesetzt --> manager-Funktionalität + TCP- und UDP-Port auf 7509
-	OWNPORT != 7509 -->	wenn OWNNAME=MANAGER	--> manager-Funktionalität + TCP-Port=OWNPORT und UDP-Port=7509
				sonst			--> KEINE manager-Funktionalität + TCP-Port=OWNPORT und KEIN UDP-Port
-	MNGLIBPORT --> UDP-Port=MNGLIBPORT WENN manager-Funktionalität übernommen wird
	
	
	runtimeserver starten mit -w ksmanagercom -w ksservtcp , also bsp.:
	
			%> OWNPORT=5556 OWNNAME=testname MNGLIBPORT=7509 ov_runtimserver -f database.ovd -w ksmanagercom -w ksservtcp

	
	
	weitere tcpconnection anlegen:
	
	-	unter ovcon tcpconnection2 anlegen
	
	-	tcpconnection2.port setzen
	
	-	tcpconnection2/managercom.sname setzen --> servername
	
	
	ACHTUNG:	bei Kommunikation mit dem klassischen ks_manager sind nun zwei Eintrage auf ein und die selbe Datenbasis vorhanden
			ein neuer ks_manager als ov-userlibrary ist in Entwicklung, der dieses Problem addressiert
