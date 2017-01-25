#HMI ecosystem 

This is the HMI system from the runtime enviroment.

An HMI application is build with objects from the cshmi library in the server and afterwards interpreted/displayed in a webbrowser with the HTML/JS files in hmiJavascript. 

This files must be served from a webserver. The easiest way is the intergrated webserver in the server. The server can load the web application in the fileformat .fbd. A stable version is provided in the templates directory of the cshmi library.
There is a tcl script `generateFBD.tcl` to convert the files into this fbd in the hmiJavascript directory.
