# HMI ecosystem 

This is the HMI system from the runtime enviroment.

An HMI application is build with objects from the cshmi library in the server and afterwards interpreted/displayed in a webbrowser with the HTML/JS files in hmiJavascript. 

This files must be served from a webserver. The easiest way is the intergrated webserver in the server. The server can load the web application in the fileformat .fbd. A stable version is provided in the templates directory of the cshmi library.
There is a tcl script `generateFBD.tcl` to convert the files into this fbd in the hmiJavascript directory.


## Blackbox
The blackbox element allows the use of plain HTML or JavaScript content. The easiest way to program the JavaScript is to use an editor with support of .d.ts  definitions like [Visual Code](https://code.visualstudio.com/). Just create a new file with .js extension and add an adjusted code like this:
````ts
/// <reference path="C:\acplt\hmiJavaScript\JavaScript\cshmimodel.d.ts" />
````
on the first line. You will get proper Intellisense for the cshmi JS API cshmimodel.
To use the JS code just add the js code in the `jsOnload` or `jsOnglobalvarchanged` variable of your Blackbox element.
