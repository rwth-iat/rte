
CSHMI - Client-Side Human-Machine Interface
-------------------------------------------

Lehrstuhl fuer Prozessleittechnik, RWTH Aachen University
Gustavo Quiros, Holger Jeromin, Stefan Schmitz
{g.quiros,h.jeromin}@plt.rwth-aachen.de

CSHMI is an ACPLT/OV model for client-side HMI, defined in the library 'cshmi'. The HMI is specified in an ACPLT/OV server by instantiating and parametrising CSHMI objects. A client can then access the model via ACPLT/KS, build the specified graphical user interface on the client side, and execute the corresponding behavior of the HMI.

Modelling Guidelines
--------------------

- Containment is used to compose the HMI.
- Graphical elements are instantianted and parametrised to represent graphics. The class cshmi/Group can be used to group graphical elements together.
- Events are instantiated and parametrised under graphical elements and groups. ClientEvents named onload are executed once when loading the HMI, 
  TimeEvents are executed periodically by the client.
  The identifier of the event object corresponds to the gesture type ('click', 'doubleclick', etc.). 
- Actions are instantiated and parametrised under events to add behavior to them. 
- KS variables used in actions refer to variables accessed through ACPLT/KS. KS paths can be both absolute or relative. Relative paths are resolved with respect to the baseKSpath of the object. This property is inherited through containment.
- Element variables used in actions refer to the variables of the containing graphical element.
- Global variables used in actions refer to global variables on the client side. Global variables are assumed to be defined when they are written for the first time.
- Event variables used in actions refer to variables of the current event.

## HMI ecosystem 

An HMI application is build with objects from the cshmi library in the server and afterwards interpreted/displayed in a webbrowser with the HTML/JS files in staticfiles. 

This files must be served from a webserver. The easiest way is the intergrated webserver in the server. The server can load the web application in the fileformat .fbd. A stable version is provided in the templates directory of the cshmi library.
There is a tcl script `generateFBD.tcl` to convert the files into this fbd in the staticfiles directory.


## Blackbox
The blackbox element allows the use of plain HTML or JavaScript content. The easiest way to program the JavaScript is to use an editor with support of .d.ts  definitions like [Visual Code](https://code.visualstudio.com/). Just create a new file with .js extension and add an adjusted code like this:
````ts
/// <reference path="C:\acplt\hmiJavaScript\JavaScript\cshmimodel.d.ts" />
````
on the first line. You will get proper Intellisense for the cshmi JS API cshmimodel.
To use the JS code just add the js code in the `jsOnload` or `jsOnglobalvarchanged` variable of your Blackbox element.
