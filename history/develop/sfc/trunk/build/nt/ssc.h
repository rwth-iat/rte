/*
*   This file was generated by the ACPLT/OV C-Code Generator.
*
*   C-Code Generator version: 1.6.4
*   ACPLT/OV library version: 1.9.0
*   Date of file generation:  15-Oct-2013 (11:35:13)
*
*   DO NOT EDIT THIS FILE!
*/

#ifndef OV_LIBRARY_ssc_H_INCLUDED
#define OV_LIBRARY_ssc_H_INCLUDED

#include "libov/ov_library.h"
#include "libov/ov_structure.h"
#include "libov/ov_class.h"
#include "libov/ov_association.h"
#include "libov/ov_variable.h"
#include "libov/ov_part.h"
#include "libov/ov_operation.h"
#include "ov.h"
#include "ksbase.h"
#include "fb.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OV_COMPILE_LIBRARY_ssc
#if OV_COMPILER_BORLAND
#define OV_VAREXTERN extern
#else
#define OV_VAREXTERN OV_DLLVAREXPORT
#endif
#define OV_FNCEXTERN OV_DLLFNCEXPORT
#else
#define OV_VAREXTERN OV_DLLVARIMPORT
#define OV_FNCEXTERN OV_DLLVARIMPORT
#endif

#define OV_PPT_ssc_actionBlocks struct OV_INST_fb_functionblock*
#define OV_CPT_ssc_actionBlocks struct OV_INST_ssc_executeFb*
#define OV_PCI_ssc_actionBlocks is_of_class_fb_functionblock
#define OV_CCI_ssc_actionBlocks is_of_class_ssc_executeFb

#define OV_PPT_ssc_nextTransitions struct OV_INST_ssc_step*
#define OV_CPT_ssc_nextTransitions struct OV_INST_ssc_transition*
#define OV_PCI_ssc_nextTransitions is_of_class_ssc_step
#define OV_CCI_ssc_nextTransitions is_of_class_ssc_transition

#define OV_PPT_ssc_previousTransitions struct OV_INST_ssc_step*
#define OV_CPT_ssc_previousTransitions struct OV_INST_ssc_transition*
#define OV_PCI_ssc_previousTransitions is_of_class_ssc_step
#define OV_CCI_ssc_previousTransitions is_of_class_ssc_transition

#define OV_BIPT_ssc_sscHeader OV_INSTPTR_fb_functionblock
#define OV_BVPT_ssc_sscHeader OV_VTBLPTR_fb_functionblock

#define OV_INSTBODY_ssc_sscHeader \
    OV_INSTBODY_fb_functionblock \
    OV_UINT v_EN; \
    OV_UINT v_woStPhase; \
    OV_UINT v_woStQualifier; \
    OV_STRING v_activeStep; \
    OV_BOOL v_terminated; \
    OV_UINT v_workingState; \
    OV_STRING v_woStText; \
    OV_BOOL v_error; \
    OV_STRING v_errorDetail; \
    OV_INST_ov_domain p_actions; \
    OV_INST_ov_domain p_transConds; \
    OV_INST_fb_task p_intask;

#define OV_CIB_ssc_sscHeader \
    OV_CIB_fb_functionblock \
    int is_of_class_ssc_sscHeader:1;

#define OV_STATICINSTBODY_ssc_sscHeader

#define OV_BIPT_ssc_step OV_INSTPTR_fb_functionblock
#define OV_BVPT_ssc_step OV_VTBLPTR_fb_functionblock

#define OV_INSTBODY_ssc_step \
    OV_INSTBODY_fb_functionblock \
    OV_UINT v_internalID; \
    OV_BOOL v_X; \
    OV_TIME_SPAN v_T; \
    OV_UINT v_phase; \
    OV_UINT v_qualifier; \
    OV_BOOL v_evTransTrigger; \
    OV_BOOL v_hasSubSsc; \
    OV_BOOL v_subSscTerminated; \
    OV_BOOL v_error; \
    OV_STRING v_errorDetail; \
    OV_INST_fb_task p_entry; \
    OV_INST_fb_task p_do; \
    OV_INST_fb_task p_trans; \
    OV_INST_fb_task p_exit;

#define OV_CIB_ssc_step \
    OV_CIB_fb_functionblock \
    int is_of_class_ssc_step:1;

#define OV_STATICINSTBODY_ssc_step

#define OV_BIPT_ssc_transition OV_INSTPTR_fb_functionblock
#define OV_BVPT_ssc_transition OV_VTBLPTR_fb_functionblock

#define OV_INSTBODY_ssc_transition \
    OV_INSTBODY_fb_functionblock \
    OV_BOOL v_result; \
    OV_BOOL v_error; \
    OV_STRING v_errorDetail;

#define OV_CIB_ssc_transition \
    OV_CIB_fb_functionblock \
    int is_of_class_ssc_transition:1;

#define OV_STATICINSTBODY_ssc_transition

#define OV_BIPT_ssc_actionBlock OV_INSTPTR_fb_functionblock
#define OV_BVPT_ssc_actionBlock OV_VTBLPTR_fb_functionblock

#define OV_INSTBODY_ssc_actionBlock \
    OV_INSTBODY_fb_functionblock \
    OV_UINT v_actionQualifier; \
    OV_STRING v_actionName; \
    OV_BOOL v_error; \
    OV_STRING v_errorDetail;

#define OV_CIB_ssc_actionBlock \
    OV_CIB_fb_functionblock \
    int is_of_class_ssc_actionBlock:1;

#define OV_STATICINSTBODY_ssc_actionBlock

#define OV_BIPT_ssc_setVariable OV_INSTPTR_ssc_actionBlock
#define OV_BVPT_ssc_setVariable OV_VTBLPTR_ssc_actionBlock

#define OV_INSTBODY_ssc_setVariable \
    OV_INSTBODY_ssc_actionBlock \
    OV_STRING v_variable; \
    OV_ANY v_value;

#define OV_CIB_ssc_setVariable \
    OV_CIB_ssc_actionBlock \
    int is_of_class_ssc_setVariable:1;

#define OV_STATICINSTBODY_ssc_setVariable

#define OV_BIPT_ssc_executeFb OV_INSTPTR_ssc_actionBlock
#define OV_BVPT_ssc_executeFb OV_VTBLPTR_ssc_actionBlock

#define OV_INSTBODY_ssc_executeFb \
    OV_INSTBODY_ssc_actionBlock

#define OV_CIB_ssc_executeFb \
    OV_CIB_ssc_actionBlock \
    int is_of_class_ssc_executeFb:1;

#define OV_STATICINSTBODY_ssc_executeFb

#define OV_BIPT_ssc_executeSsc OV_INSTPTR_ssc_executeFb
#define OV_BVPT_ssc_executeSsc OV_VTBLPTR_ssc_executeFb

#define OV_INSTBODY_ssc_executeSsc \
    OV_INSTBODY_ssc_executeFb \
    OV_UINT v_enableSsc;

#define OV_CIB_ssc_executeSsc \
    OV_CIB_ssc_executeFb \
    int is_of_class_ssc_executeSsc:1;

#define OV_STATICINSTBODY_ssc_executeSsc

OV_TYPEDEF_INSTANCE(ssc_sscHeader);
OV_TYPEDEF_INSTANCE(ssc_step);
OV_TYPEDEF_INSTANCE(ssc_transition);
OV_TYPEDEF_INSTANCE(ssc_actionBlock);
OV_TYPEDEF_INSTANCE(ssc_setVariable);
OV_TYPEDEF_INSTANCE(ssc_executeFb);
OV_TYPEDEF_INSTANCE(ssc_executeSsc);

#include "ssc.ovf"

OV_FNCEXTERN OV_FNC_CONSTRUCTOR ssc_sscHeader_constructor;
OV_FNCEXTERN FB_FNC_TYPEMETHOD ssc_sscHeader_typemethod;
OV_FNCEXTERN SSC_FNC_RESETSSC ssc_sscHeader_resetSsc;

OV_FNCEXTERN OV_FNC_CONSTRUCTOR ssc_step_constructor;
OV_FNCEXTERN FB_FNC_TYPEMETHOD ssc_step_typemethod;
OV_FNCEXTERN SSC_FNC_RESETSTEP ssc_step_resetStep;

OV_FNCEXTERN OV_FNC_CONSTRUCTOR ssc_transition_constructor;
OV_FNCEXTERN FB_FNC_TYPEMETHOD ssc_transition_typemethod;

OV_FNCEXTERN OV_FNC_CONSTRUCTOR ssc_actionBlock_constructor;
OV_FNCEXTERN SSC_FNC_SETACTIONNAME ssc_actionBlock_setActionName;

OV_FNCEXTERN FB_FNC_TYPEMETHOD ssc_setVariable_typemethod;
OV_FNCEXTERN SSC_FNC_SETACTIONNAME ssc_setVariable_setActionName;

OV_FNCEXTERN FB_FNC_TYPEMETHOD ssc_executeFb_typemethod;
OV_FNCEXTERN SSC_FNC_SETACTIONNAME ssc_executeFb_setActionName;
OV_FNCEXTERN SSC_FNC_CHECKACTION ssc_executeFb_checkAction;

OV_FNCEXTERN FB_FNC_TYPEMETHOD ssc_executeSsc_typemethod;
OV_FNCEXTERN OV_FNC_DESTRUCTOR ssc_executeSsc_destructor;

#define OV_VTBLBODY_ssc_sscHeader \
    OV_VTBLBODY_fb_functionblock \
    SSC_FNC_RESETSSC* m_resetSsc;

#define OV_VTBLBODY_ssc_step \
    OV_VTBLBODY_fb_functionblock \
    SSC_FNC_RESETSTEP* m_resetStep;

#define OV_VTBLBODY_ssc_transition \
    OV_VTBLBODY_fb_functionblock

#define OV_VTBLBODY_ssc_actionBlock \
    OV_VTBLBODY_fb_functionblock \
    SSC_FNC_SETACTIONNAME* m_setActionName;

#define OV_VTBLBODY_ssc_setVariable \
    OV_VTBLBODY_ssc_actionBlock

#define OV_VTBLBODY_ssc_executeFb \
    OV_VTBLBODY_ssc_actionBlock \
    SSC_FNC_CHECKACTION* m_checkAction;

#define OV_VTBLBODY_ssc_executeSsc \
    OV_VTBLBODY_ssc_executeFb

OV_TYPEDEF_VTABLE(ssc_sscHeader);
OV_TYPEDEF_VTABLE(ssc_step);
OV_TYPEDEF_VTABLE(ssc_transition);
OV_TYPEDEF_VTABLE(ssc_actionBlock);
OV_TYPEDEF_VTABLE(ssc_setVariable);
OV_TYPEDEF_VTABLE(ssc_executeFb);
OV_TYPEDEF_VTABLE(ssc_executeSsc);

OV_DLLFNCEXPORT OV_RESULT ssc_actionBlock_actionQualifier_set(
    OV_INSTPTR_ssc_actionBlock pobj,
    const OV_UINT value
);

OV_DLLFNCEXPORT OV_RESULT ssc_actionBlock_actionName_set(
    OV_INSTPTR_ssc_actionBlock pobj,
    const OV_STRING value
);

OV_DLLFNCEXPORT OV_RESULT ssc_setVariable_variable_set(
    OV_INSTPTR_ssc_setVariable pobj,
    const OV_STRING value
);

OV_DLLFNCEXPORT OV_RESULT ssc_setVariable_value_set(
    OV_INSTPTR_ssc_setVariable pobj,
    const OV_ANY *pvalue
);

OV_DECL_LINK(ssc_actionBlocks);
OV_DECL_UNLINK(ssc_actionBlocks);
OV_DECL_GETACCESS(ssc_actionBlocks);

OV_DECL_LINK(ssc_nextTransitions);
OV_DECL_UNLINK(ssc_nextTransitions);
OV_DECL_GETACCESS(ssc_nextTransitions);

OV_DECL_LINK(ssc_previousTransitions);
OV_DECL_UNLINK(ssc_previousTransitions);
OV_DECL_GETACCESS(ssc_previousTransitions);

extern OV_CLASS_DEF OV_CLASS_DEF_ssc_sscHeader;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ssc_sscHeader;

extern OV_CLASS_DEF OV_CLASS_DEF_ssc_step;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ssc_step;

extern OV_CLASS_DEF OV_CLASS_DEF_ssc_transition;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ssc_transition;

extern OV_CLASS_DEF OV_CLASS_DEF_ssc_actionBlock;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ssc_actionBlock;

extern OV_CLASS_DEF OV_CLASS_DEF_ssc_setVariable;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ssc_setVariable;

extern OV_CLASS_DEF OV_CLASS_DEF_ssc_executeFb;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ssc_executeFb;

extern OV_CLASS_DEF OV_CLASS_DEF_ssc_executeSsc;
OV_VAREXTERN OV_INSTPTR_ov_class pclass_ssc_executeSsc;

extern OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_ssc_actionBlocks;
OV_VAREXTERN OV_INSTPTR_ov_association passoc_ssc_actionBlocks;

extern OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_ssc_nextTransitions;
OV_VAREXTERN OV_INSTPTR_ov_association passoc_ssc_nextTransitions;

extern OV_ASSOCIATION_DEF OV_ASSOCIATION_DEF_ssc_previousTransitions;
OV_VAREXTERN OV_INSTPTR_ov_association passoc_ssc_previousTransitions;

extern OV_LIBRARY_DEF OV_LIBRARY_DEF_ssc;

OV_RESULT ov_library_setglobalvars_ssc(void);

OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_ssc(void);

#undef OV_VAREXTERN
#undef OV_FNCEXTERN

#ifdef __cplusplus
}
#endif

#endif

/*
*   End of file
*/
