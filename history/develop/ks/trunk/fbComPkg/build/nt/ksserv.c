/*
*   This file was generated by the ACPLT/OV C-Code Generator.
*
*   C-Code Generator version: 1.6.4
*   ACPLT/OV library version: 1.8.0
*   Date of file generation:  12-Jul-2012 (17:28:22)
*
*   DO NOT EDIT THIS FILE!
*/

#ifndef OV_COMPILE_LIBRARY_ksserv
#define OV_COMPILE_LIBRARY_ksserv
#endif

#include "ksserv.h"

OV_DLLVAREXPORT OV_INSTPTR_ov_class pclass_ksserv_comcontainer = NULL;
OV_DLLVAREXPORT OV_INSTPTR_ov_class pclass_ksserv_RootComTask = NULL;
OV_DLLVAREXPORT OV_INSTPTR_ov_class pclass_ksserv_ComTask = NULL;
OV_DLLVAREXPORT OV_INSTPTR_ov_class pclass_ksserv_Client = NULL;

OV_DLLVAREXPORT OV_INSTPTR_ov_association passoc_ksserv_AssocComTaskList = NULL;
OV_DLLVAREXPORT OV_INSTPTR_ov_association passoc_ksserv_AssocCurrentClient = NULL;

OV_VTBL_ksserv_comcontainer OV_VTABLE_ksserv_comcontainer = {
    ov_object_constructor,
    ov_object_checkinit,
    ov_object_destructor,
    ov_object_startup,
    ov_object_shutdown,
    ov_object_getaccess,
    ov_object_getflags,
    ov_object_getcomment,
    ov_object_gettechunit,
    ov_object_getvar,
    ov_object_setvar
};

OV_VTBL_ksserv_RootComTask OV_VTABLE_ksserv_RootComTask = {
    ov_object_constructor,
    ov_object_checkinit,
    ov_object_destructor,
    ksserv_RootComTask_startup,
    ksserv_RootComTask_shutdown,
    ov_object_getaccess,
    ov_object_getflags,
    ov_object_getcomment,
    ov_object_gettechunit,
    ov_object_getvar,
    ov_object_setvar
};

OV_VTBL_ksserv_ComTask OV_VTABLE_ksserv_ComTask = {
    ksserv_ComTask_constructor,
    ov_object_checkinit,
    ksserv_ComTask_destructor,
    ov_object_startup,
    ov_object_shutdown,
    ov_object_getaccess,
    ov_object_getflags,
    ov_object_getcomment,
    ov_object_gettechunit,
    ov_object_getvar,
    ov_object_setvar,
    ksserv_ComTask_calcExec,
    ksserv_ComTask_typemethod
};

OV_VTBL_ksserv_Client OV_VTABLE_ksserv_Client = {
    ksserv_ComTask_constructor,
    ov_object_checkinit,
    ksserv_ComTask_destructor,
    ov_object_startup,
    ov_object_shutdown,
    ov_object_getaccess,
    ov_object_getflags,
    ov_object_getcomment,
    ov_object_gettechunit,
    ov_object_getvar,
    ov_object_setvar,
    ksserv_ComTask_calcExec,
    ksserv_ComTask_typemethod,
    ksserv_Client_setThisAsCurrent,
    ksserv_Client_unsetThisAsCurrent
};


OV_CLASS_DEF OV_CLASS_DEF_ksserv_comcontainer = {
    &OV_CLASS_DEF_ksserv_RootComTask,
    "comcontainer",
    "ov/domain",
    1,
    0,
    "General Communication domain,nothing specific, source of the CurrentClient Assoc",
    Ov_GetInstSize(ksserv_comcontainer),
    Ov_GetStaticInstSize(ksserv_comcontainer),
    (OV_POINTER)&OV_VTABLE_ksserv_comcontainer,
    (OV_VARIABLE_DEF*)0,
    (OV_PART_DEF*)0,
    (OV_OPERATION_DEF*)0
};


OV_VARIABLE_DEF OV_VARIABLE_DEF_ksserv_RootComTask[] = {{
    &OV_VARIABLE_DEF_ksserv_RootComTask[1],
    "cycsecs",
    1,
    3,
    16,
    (OV_STRING)NULL,
    (OV_STRING)NULL,
    1*sizeof(OV_INT),
    offsetof(OV_INST_ksserv_RootComTask, v_cycsecs),
    0,
    (OV_STRING)NULL,
    "seconds for scheduler registration. Please note that we do not calculate the among of time spent - root task just pauses for this time intervall.",
    (OV_FNCPTR_GET)ksserv_RootComTask_cycsecs_get,
    (OV_FNCPTR_SET)ksserv_RootComTask_cycsecs_set
}, {
    (OV_VARIABLE_DEF*)0,
    "cycusecs",
    1,
    3,
    16,
    (OV_STRING)NULL,
    (OV_STRING)NULL,
    1*sizeof(OV_INT),
    offsetof(OV_INST_ksserv_RootComTask, v_cycusecs),
    0,
    (OV_STRING)NULL,
    "usecs for scheduler registration",
    (OV_FNCPTR_GET)ksserv_RootComTask_cycusecs_get,
    (OV_FNCPTR_SET)ksserv_RootComTask_cycusecs_set
}};

OV_OPERATION_DEF OV_OPERATION_DEF_ksserv_RootComTask[] = {{
    &OV_OPERATION_DEF_ksserv_RootComTask[1],
    "startup",
    0,
    "OV_FNC_STARTUP"
}, {
    (OV_OPERATION_DEF*)0,
    "shutdown",
    0,
    "OV_FNC_SHUTDOWN"
}};

OV_CLASS_DEF OV_CLASS_DEF_ksserv_RootComTask = {
    &OV_CLASS_DEF_ksserv_ComTask,
    "RootComTask",
    "ov/domain",
    1,
    0,
    "root communication task - cycling all connected tasks. ",
    Ov_GetInstSize(ksserv_RootComTask),
    Ov_GetStaticInstSize(ksserv_RootComTask),
    (OV_POINTER)&OV_VTABLE_ksserv_RootComTask,
    &OV_VARIABLE_DEF_ksserv_RootComTask[0],
    (OV_PART_DEF*)0,
    &OV_OPERATION_DEF_ksserv_RootComTask[0]
};


OV_VARIABLE_DEF OV_VARIABLE_DEF_ksserv_ComTask[] = {{
    &OV_VARIABLE_DEF_ksserv_ComTask[1],
    "actimode",
    1,
    3,
    16,
    (OV_STRING)NULL,
    (OV_STRING)NULL,
    1*sizeof(OV_INT),
    offsetof(OV_INST_ksserv_ComTask, v_actimode),
    0,
    (OV_STRING)NULL,
    "0=NonActive, 1=Active",
    (OV_FNCPTR_GET)ksserv_ComTask_actimode_get,
    (OV_FNCPTR_SET)ksserv_ComTask_actimode_set
}, {
    &OV_VARIABLE_DEF_ksserv_ComTask[2],
    "cycIntervalCount",
    1,
    1,
    16,
    (OV_STRING)NULL,
    (OV_STRING)NULL,
    1*sizeof(OV_INT),
    offsetof(OV_INST_ksserv_ComTask, v_cycIntervalCount),
    0,
    (OV_STRING)NULL,
    "current wait cycles until next execution",
    (OV_FNCPTR_GET)ksserv_ComTask_cycIntervalCount_get,
    NULL
}, {
    &OV_VARIABLE_DEF_ksserv_ComTask[3],
    "cycInterval",
    1,
    3,
    16,
    (OV_STRING)NULL,
    (OV_STRING)NULL,
    1*sizeof(OV_INT),
    offsetof(OV_INST_ksserv_ComTask, v_cycInterval),
    0,
    (OV_STRING)NULL,
    "execution intervall of cycles of RootTasks",
    (OV_FNCPTR_GET)ksserv_ComTask_cycInterval_get,
    (OV_FNCPTR_SET)ksserv_ComTask_cycInterval_set
}, {
    (OV_VARIABLE_DEF*)0,
    "NextExecTime",
    1,
    0,
    49,
    (OV_STRING)NULL,
    (OV_STRING)NULL,
    1*sizeof(OV_TIME),
    offsetof(OV_INST_ksserv_ComTask, v_NextExecTime),
    0,
    (OV_STRING)NULL,
    "time of next execution (if cycInterval !=0)",
    NULL,
    NULL
}};

OV_OPERATION_DEF OV_OPERATION_DEF_ksserv_ComTask[] = {{
    &OV_OPERATION_DEF_ksserv_ComTask[1],
    "constructor",
    0,
    "OV_FNC_CONSTRUCTOR"
}, {
    &OV_OPERATION_DEF_ksserv_ComTask[2],
    "destructor",
    0,
    "OV_FNC_DESTRUCTOR"
}, {
    &OV_OPERATION_DEF_ksserv_ComTask[3],
    "calcExec",
    0,
    "FNC_CALCEXEC"
}, {
    (OV_OPERATION_DEF*)0,
    "typemethod",
    0,
    "FNC_TYPEMETHOD"
}};

OV_CLASS_DEF OV_CLASS_DEF_ksserv_ComTask = {
    &OV_CLASS_DEF_ksserv_Client,
    "ComTask",
    "ov/domain",
    0,
    0,
    "communication task - get called typemethod every $cycInterval by root task ",
    Ov_GetInstSize(ksserv_ComTask),
    Ov_GetStaticInstSize(ksserv_ComTask),
    (OV_POINTER)&OV_VTABLE_ksserv_ComTask,
    &OV_VARIABLE_DEF_ksserv_ComTask[0],
    (OV_PART_DEF*)0,
    &OV_OPERATION_DEF_ksserv_ComTask[0]
};


OV_VARIABLE_DEF OV_VARIABLE_DEF_ksserv_Client[] = {{
    &OV_VARIABLE_DEF_ksserv_Client[1],
    "sourceAdr",
    1,
    1,
    48,
    (OV_STRING)NULL,
    (OV_STRING)NULL,
    1*sizeof(OV_STRING),
    offsetof(OV_INST_ksserv_Client, v_sourceAdr),
    0,
    (OV_STRING)NULL,
    "Source adr of client (e.g. IP)",
    (OV_FNCPTR_GET)ksserv_Client_sourceAdr_get,
    NULL
}, {
    &OV_VARIABLE_DEF_ksserv_Client[2],
    "ovticket",
    1,
    1,
    48,
    (OV_STRING)NULL,
    (OV_STRING)NULL,
    1*sizeof(OV_STRING),
    offsetof(OV_INST_ksserv_Client, v_ovticket),
    0,
    (OV_STRING)NULL,
    "Ticket - ONLY set during call from client - thus invisible!",
    (OV_FNCPTR_GET)ksserv_Client_ovticket_get,
    NULL
}, {
    (OV_VARIABLE_DEF*)0,
    "receivedCalls",
    1,
    1,
    16,
    (OV_STRING)NULL,
    (OV_STRING)NULL,
    1*sizeof(OV_INT),
    offsetof(OV_INST_ksserv_Client, v_receivedCalls),
    0,
    (OV_STRING)NULL,
    "Number of received calls from this client",
    (OV_FNCPTR_GET)ksserv_Client_receivedCalls_get,
    NULL
}};

OV_OPERATION_DEF OV_OPERATION_DEF_ksserv_Client[] = {{
    &OV_OPERATION_DEF_ksserv_Client[1],
    "setThisAsCurrent",
    0,
    "FNC_SETCURRENT"
}, {
    (OV_OPERATION_DEF*)0,
    "unsetThisAsCurrent",
    0,
    "FNC_SETCURRENT"
}};

OV_CLASS_DEF OV_CLASS_DEF_ksserv_Client = {
    (OV_CLASS_DEF*)0,
    "Client",
    "ksserv/ComTask",
    0,
    0,
    "General clients ",
    Ov_GetInstSize(ksserv_Client),
    Ov_GetStaticInstSize(ksserv_Client),
    (OV_POINTER)&OV_VTABLE_ksserv_Client,
    &OV_VARIABLE_DEF_ksserv_Client[0],
    (OV_PART_DEF*)0,
    &OV_OPERATION_DEF_ksserv_Client[0]
};

OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_ksserv_AssocComTaskList = {
    &OV_ASSOCIATION_DEF_ksserv_AssocCurrentClient,
    "AssocComTaskList",
    1,
    0,
    "comtaskchild",
    "ksserv/RootComTask",
    "rootcomtask",
    "ksserv/ComTask",
    (OV_STRING)NULL,
    (OV_STRING)NULL,
    0,
    0,
    0,
    0,
    (OV_FNCPTR_LINK)ksserv_AssocComTaskList_link,
    (OV_FNCPTR_UNLINK)ksserv_AssocComTaskList_unlink,
    (OV_FNCPTR_GETACCESS)ksserv_AssocComTaskList_getaccess
};

OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_ksserv_AssocCurrentClient = {
    (OV_ASSOCIATION_DEF*)0,
    "AssocCurrentClient",
    3,
    0,
    "currentClient",
    "ksserv/comcontainer",
    "connections",
    "ksserv/Client",
    (OV_STRING)NULL,
    (OV_STRING)NULL,
    0,
    0,
    0,
    0,
    (OV_FNCPTR_LINK)ksserv_AssocCurrentClient_link,
    (OV_FNCPTR_UNLINK)ksserv_AssocCurrentClient_unlink,
    (OV_FNCPTR_GETACCESS)ksserv_AssocCurrentClient_getaccess
};

OV_LIBRARY_DEF OV_LIBRARY_DEF_ksserv = {
    "ksserv",
    "0.1",
    "1.8.0",
    "Henning Mersch",
    "Lehrstuhl fuer Prozessleittechnik",
    "general communication components - independant of communcation system",
    (OV_STRUCTURE_DEF*)0,
    &OV_CLASS_DEF_ksserv_comcontainer,
    &OV_ASSOCIATION_DEF_ksserv_AssocComTaskList,
    ov_library_setglobalvars_ksserv
};

#include "libov/ov_macros.h"

OV_RESULT ov_library_setglobalvars_ksserv(void) {
    OV_INSTPTR_ov_library plib;
    OV_INSTPTR_ov_variable pvar;

    Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
        if(!strcmp(plib->v_identifier, "ksserv")) {
            pclass_ksserv_comcontainer = Ov_SearchChildEx(ov_containment, plib, "comcontainer", ov_class);
            if(!pclass_ksserv_comcontainer) {
                return OV_ERR_GENERIC;
            }
            pclass_ksserv_RootComTask = Ov_SearchChildEx(ov_containment, plib, "RootComTask", ov_class);
            if(!pclass_ksserv_RootComTask) {
                return OV_ERR_GENERIC;
            }
            pvar = Ov_SearchChildEx(ov_containment, pclass_ksserv_RootComTask, "cycsecs", ov_variable);
            pvar->v_initialvalue.value.vartype = OV_VT_INT;
            pvar->v_initialvalue.value.valueunion.val_int = 0 ;
            pvar = Ov_SearchChildEx(ov_containment, pclass_ksserv_RootComTask, "cycusecs", ov_variable);
            pvar->v_initialvalue.value.vartype = OV_VT_INT;
            pvar->v_initialvalue.value.valueunion.val_int = 1000 ;
            pclass_ksserv_ComTask = Ov_SearchChildEx(ov_containment, plib, "ComTask", ov_class);
            if(!pclass_ksserv_ComTask) {
                return OV_ERR_GENERIC;
            }
            pvar = Ov_SearchChildEx(ov_containment, pclass_ksserv_ComTask, "actimode", ov_variable);
            pvar->v_initialvalue.value.vartype = OV_VT_INT;
            pvar->v_initialvalue.value.valueunion.val_int = 1 ;
            pvar = Ov_SearchChildEx(ov_containment, pclass_ksserv_ComTask, "cycIntervalCount", ov_variable);
            pvar->v_initialvalue.value.vartype = OV_VT_INT;
            pvar->v_initialvalue.value.valueunion.val_int = 5 ;
            pvar = Ov_SearchChildEx(ov_containment, pclass_ksserv_ComTask, "cycInterval", ov_variable);
            pvar->v_initialvalue.value.vartype = OV_VT_INT;
            pvar->v_initialvalue.value.valueunion.val_int = 5 ;
            pclass_ksserv_Client = Ov_SearchChildEx(ov_containment, plib, "Client", ov_class);
            if(!pclass_ksserv_Client) {
                return OV_ERR_GENERIC;
            }
            passoc_ksserv_AssocComTaskList = Ov_SearchChildEx(ov_containment, plib, "AssocComTaskList", ov_association);
            if(!passoc_ksserv_AssocComTaskList) {
                return OV_ERR_GENERIC;
            }
            passoc_ksserv_AssocCurrentClient = Ov_SearchChildEx(ov_containment, plib, "AssocCurrentClient", ov_association);
            if(!passoc_ksserv_AssocCurrentClient) {
                return OV_ERR_GENERIC;
            }
            return OV_ERR_OK;
        }
    }
    return OV_ERR_GENERIC;
}

OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_ksserv(void) {
    return &OV_LIBRARY_DEF_ksserv;
}

/*
*   End of file
*/
