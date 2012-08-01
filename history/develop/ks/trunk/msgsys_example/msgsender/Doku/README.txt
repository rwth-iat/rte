!!!VORABVERSION!!!

************************************
I. Verwendung
************************************

Der Sender stellt den Service da, der dem Receiver eine Nachricht sendet und daraufhin
von diesem ein Antwortnachricht erhält.

1. Der Benutzer erstellt über iFBSpro in der Outbox eine Nachricht vom Typ Message.

2. Diese Nachricht ist mit Defaultwerten bestückt, benötigt aber noch z.B.: Stringdaten
   zum vesenden.

3. Das Senderobjekt besitzt eine "sendCMD"-Variable, diese einfach mit einem beliebigen    Wert setzen und die Nachricht wird darauf hin an den Receiver versandt. 

-> Der Receiver erhält diese Nachricht und wird darauf hin mit einer Nachricht mit dem
   Inhalt "I got your Message!" Antworten. 