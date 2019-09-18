#Kommunikation im ACPLT Server
Die ursprüngliche Kommunikation des Systems läuft über das ACPLT/KS Protokoll. Mittlerweile ist über die Bibliothek opcua aber auch ein OPCUA Server problemlos möglich.

#Anwendungsbereich von ACPLT/KS
ACPLT/KS ist ein Kommunikationskonzept zum einfachen Austausch von aktuellen Zustandswerten, Modellinformationen und zum Übertragen von Modelltransformationsbefehlen. ACPLT/KS realisiert den Austausch in einzelnen Kommunikationsbeziehungen nach dem Client-Serverprinzip. ACPLT/KS betrachtet konzeptionell nur die einzelne Beziehung, lässt aber beliebig viele voneinander unabhängige Beziehungen zu. In der Umsetzung kann ein Peer daher gleichzeitig mehrere Beziehungen unterhalten in denen er auch in unterschiedlichen Rollen (Client/Server) auftreten kann. 

In der einzelnen Beziehung realisiert ACPLT/KS eine 1:1 –Beziehung zwischen einem Client und einem Server. Bei jedem Datenaustausch schickt der Client einen Request und der Server antwortet mit genau einem Response, dann ist der Datenaustausch beendet. Der Client kann den nächsten Request erst senden wenn der Response eingetroffen ist (außer im Fehlerfall nach Timeout). Die Dienste die durch die ACPLT/KS Requests im Server angestoßen werden können als atomar betrachtet werden, d. h: ihre Ausführung und die Generierung der Response erfolgt sofort und ohne Beeinträchtigung der Echtzeit in der Serveranwendung. 

(Bemerkung: Dies ist durch die einfache Art der Dienste in fast allen „vernünftigen“ Anwendungsfällen gegeben, zur Sicherstellung müssen allerdings Restriktionen wie Pfadtiefe, Anzahl der in einem Request gelesenen Variablen, Namenslängen usw. berücksichtigt werden. In der NAMUR-Empfehlung NE139 sind einige dieser Beschränkungen beschrieben. Es ist jedoch zu bemerken, dass in heutigen Systemen im Allgemeinen die Beschränkungen nicht zum Tragen kommen.)

Mit diesem Konzept ist die Kommunikation von Seiten des Klienten synchron und von Seiten des Servers zustandslos.

ACPLT/KS ist für viele Anwendungen ausreichend. Es ermöglicht insbesondere die Ankopplung an die Leitsysteme und den effizienten Datenaustausch zwischen Automatisierungskomponenten. Für höherwertige und komplexere Kommunikationsanforderungen dienen das ACPLT- Message-System (ACPLT/KS-M) und das ACPLT Service-System (ACPLT/KS-S). Beide können direkt auf das Basissystem ACPLT/KS aufgesetzt werden.

#Aufbau von ACPLT/KS

ACPLT/KS besteht aus drei Protokollschichten. Auf der obersten Schicht befindet sich die funktionale Semantik. Diese ist einheitlich festgelegt und charakterisiert die Kommunikationsfunktionalität von ACPLT/KS. Auf der darunterliegenden Schicht erfolgt die Kodierung der zu übertragenden Informationen in eine Protokollsprache. Hier gibt es mehrere Varianten. Zurzeit werden ein binäres Format (ksXDR) und eine textbasierte Kodierungen (ksHTTP) mit mehreren Varianten (plain / tcl / xml) unterstützt. Auf der untersten Ebene werden die ACPLT/KS Aufrufe auf die entsprechenden Bussysteme abgebildet. Zurzeit gibt es nur eine Abbildung auf TCP. Abbildungen z.B. auf CAN-Tunnel oder Profibus-Tunnel wären jedoch ebenfalls einfach realisierbar.

Für viele Systeme (Leitsysteme, PIMS, Archivsysteme...) sind entsprechende Serverkomponenten vorhanden. (Die ACPLT/KS-Server sind konfigurationsfrei, sie müssen nur einmal installiert sein, dann steht die gesamte Funktionalität – auch bei nachträglichen Änderungen der Modelle im Zielsystem - immer vollständig zur Verfügung)


#Pfadsyntax in KS

Volle Pfade werden im KS folgendermaßen formuliert:

`//dev/ov_server_a/vendor/server_description`

Dies bezeichnet eine Variable namens /vendor/server_description auf einem Server namens ov_server_a dessen Zugriffsport man über einen KS-Manager auf Port 7509 unter dem Rechner-namen dev findet.

Das Adressziel ist das technologische Ziel und unabhängig von der Art der Kodierung (ksXDR und ksHTTP). Die Client-Anwendung kann wählen, über welches Protokoll sie den Server erreichen möchte. Die Wege eines Zugriffs sind jedoch intern je nach gewähltem Protokoll unterschiedlich.

Ist der Port des Servers bekannt, so kann die Abfrage beim KSMANAGER gespart und der Server direkt adressiert werden. Die Syntax lautet dann zum Beispiel für den Port 240678:

`//dev/ov_server_a:240678/vendor/server_description`

Eine Angabe des Ports des MANAGERS

`//dev:7509/ov_server_a:240678/vendor/server_description`

kann erfolgen, ist dann jedoch (wie der Servername selbst) ohne Bedeutung. Der Manager läuft immer auf Port 7509!

Gültige Zugriffe auf den Manager sind:

````
//dev:7509/MANAGER:7509/vendor/server_description
//dev/MANAGER:7509/vendor/server_description
//dev:7509/MANAGER/vendor/server_description
//dev/MANAGER/vendor/server_description
````

##ksXDR
Eine Anzeige aller vorhandenen Server wird über ein GetPP (nicht in kshttp realisiert) auf die Domain /servers des Managers (Port 7509 des Rechners) ausgelöst. Dabei wird davon ausgegangen, dass der Manager läuft. Soll ein spezifischer Server angefragt werden, so kontaktiert die Applikation den Manager und setzt dort einen Befehl GETSERVER ab um den TCP-Port zu erhalten.

Ein Zugriff auf einen Server ohne Manager ist möglich, aber wird von vielen Applikationen nicht unterstützt.

##ksHTTP
Eine Anzeige aller vorhandenen Server wird über ein GetEP auf die Domain /servers des Managers (Port 7509 des Rechners) ausgelöst. Dabei wird davon ausgegangen, dass der MANAGER läuft. 

Soll ein spezifischer Server angefragt werden, so kontaktiert die Applikation den Manager und setzt dort einen Befehl getServer ab um den TCP-Port zu erhalten.

Wird ein Port in der Adresse angegeben, so wird kein MANAGER gefragt, sondern direkt eine Verbindung auf diesem Port aufgebaut.

###Beispiele für ksHTTP
`//dev/fb_control/vendor/server_description`

Da KEIN Port angegeben wurde muss der MANAGER (auf Port 7509) nach dem Port für fb_control gefragt werden

`//dev/fb_control:8080/vendor/server_description`

Da ein Port für den Server angegeben ist, greift das Programm direkt auf den Port 8080 zu und fragt keinen MANAGER um diesen zu erfahren.

Intern muss ein http-KS Client folgende http Ressourcen aufrufen:

````
http://dev:7509/getServers?servername=fb_control => 8080
http://dev:8080/getVar?path=vendor/server_description
````
