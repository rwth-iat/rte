
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
- Actions are instantiated and parametrised under graphical elements to add behavior to them. Actions may be defined as one-shot (executed once when loading the HMI) or as cyclic (executed periodically by the client). A top-level action (i.e. directly under a graphical element) executes all of its sub-actions according to it (i.e. cyclic or one-shot), while the execution configuration of the sub-actions is ignored.
- Gestures are instantiated and parametrised under graphical elements to add user interaction to them. The identifier of the gesture object corresponds to the gesture type ('click', 'mousemove', etc.). Actions are instantiated and parametrised under gestures in order to define the behavior of the gesture (i.e. the event handler).
- KS variables used in actions refer to variables accessed through ACPLT/KS. KS paths can be both absolute or relative. Relative paths are resolved with respect to the base KS path of the object. This property is inherited through containment.
- Element variables used in actions refer to the variables of the containing graphical element.
- Global variables used in actions refer to global variables on the client side. Global variables are assumed to be defined when they are written for the first time.
- Event variables used in actions refer to variables of the current event. Events are only accessible within gesture.
