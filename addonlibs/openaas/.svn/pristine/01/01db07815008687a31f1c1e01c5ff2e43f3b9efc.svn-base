Die OV-Bibliothek fbcomlib wird zur Kommunikation zwischen verschiedenen KS-Servern benutzt. Sie bildet die volle Funktionalität des KS Protokolls ab. Zur Kommunikation wird die Bibliothek ksapi genutzt, welche die gleiche Funktionalität bereitstellt, diese jedoch nicht in Funktionsbausteine koppelt. Benötigt man keine Anbindung an Funktionsbausteine durch Verbindungen, so ist die Verwendung von ksapi zu erwägen.

All functionblocks of this library inherit some variables from the class FBComCommon. Their primary focus is configuring the communication.
Inputs are defined in FB to have the flag „i“, outputs are marked with „o“.

Flag	| Name	| VarTyp	| description
-----|-----|-----|-----
i	| host	| STRING	| Name of the target computer
i	| server	| STRING	| Name of the target KS server
i	| path	| STRING	| Name of the variable to manipulate
i	| timeout	| UINT	| Timeout for request in seconds
i	| doCyclic	| BOOL	| Set to true to cyclically execute the command
i	| retryAfter	| UINT	| In case of error retry after xx seconds. Only active if doCyclic is TRUE.
i	| doSend	| BOOL	| Setting this to TRUE starts the request
o	| stateString	| STRING	| This is the answer state (as human readable string)
o	| state	| INT	| Answer as int: 0: initial * 1: request sent, awaiting answer * 2: everything OK * 4: invalid input * 8: internal error * 16: ksapi error * 32: external error * 64: timeout * 128: unknown error
o	| errorCode	| UINT	| local ov-ErrorCode
o	| opResult	| UINT	| Result of requested operation as ov errorcode
n	| requestSendTime	| TIME	| Time when request was sent
n	| sendRequested	| BOOL	| True means a send was requested
 