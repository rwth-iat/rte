
/******************************************************************************
*
*   FILE
*   ----
*   localConnection.c
*
*   History
*   -------
*   2011-03-04   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif


#include "fb.h"
#include "libov/ov_macros.h"
#include "ov_call_macros_10.h"
#include "fb_macros.h"

OV_BOOL 
fb_connection_getelements
(OV_INSTPTR_fb_connection	pconn);

int 
fb_connectiontype_implemented
(OV_VAR_TYPE typ);

OV_DLLFNCEXPORT void
fb_localconnection_trigger
(OV_INSTPTR_fb_connection	pconn)
{
  OV_INSTPTR_fb_localconnection pinst = 
    Ov_StaticPtrCast (fb_localconnection, pconn);
  OV_ANY varcurrprops;
  OV_INSTPTR_fb_functionblock psourcefb, ptargetfb;
  OV_INSTPTR_fb_functionchart psourcefc, ptargetfc;
  OV_INSTPTR_ov_object psourceobj, ptargetobj;
  OV_INSTPTR_fb_variable psourcevar, ptargetvar;

  /* Do nothing if connection is deactivated */

  if (pinst->v_on == FALSE) return;
  
  /* Get pointers to function blocks */
  
  psourcefb = Ov_GetParent (fb_outputconnections, pinst);
  ptargetfb = Ov_GetParent (fb_inputconnections, pinst);
  psourcefc = Ov_GetParent (fb_initialconnections, pinst);
  ptargetfc = Ov_GetParent (fb_finalconnections, pinst);
  
  if ((pinst->v_sourceelem.elemtype == OV_ET_NONE) ||
      (pinst->v_targetelem.elemtype == OV_ET_NONE))
  {
    if (! fb_connection_getelements (pconn))
    {
      /* Deactivate connection */
      pinst->v_on = FALSE;
      
      if (psourcefb) psourcefb->v_ErrState = 1;
      if (ptargetfb) ptargetfb->v_ErrState = 1;
      if (psourcefc) psourcefc->v_ErrState = 1;
      if (ptargetfc) ptargetfc->v_ErrState = 1;
      
      return;
    }
  }

  /* Get source and target variables, if any */

  psourcevar = Ov_GetParent (fb_sourcevariable, pinst);
  ptargetvar = Ov_GetChild (fb_targetvariable, pinst);

  /* Determine source and target objects */

  psourceobj = NULL;
  ptargetobj = NULL;
  if (psourcefb) psourceobj = Ov_PtrUpCast (ov_object, psourcefb);
  if (ptargetfb) ptargetobj = Ov_PtrUpCast (ov_object, ptargetfb);
  if (psourcevar) psourceobj = Ov_PtrUpCast (ov_object, psourcevar);
  if (ptargetvar) ptargetobj = Ov_PtrUpCast (ov_object, ptargetvar);
  
  /* Get source value and set target value */
  
  if (Ov_Fail (Ov_Call2 (ov_object, psourceobj, getvar,
                         &pconn->v_sourceelem, &varcurrprops)))
  {
    /* Deactivate connection */
    pinst->v_on = FALSE;
    return;
  }
  if (Ov_Fail (Ov_Call2 (ov_object, ptargetobj, setvar,
                         &pconn->v_targetelem, &varcurrprops)))
  {
    /* Deactivate connection */
    pinst->v_on = FALSE;
    return;
  }

}

static OV_INSTPTR_fb_variable
getVariable
(OV_INSTPTR_fb_functionchart pfc, OV_STRING id)
{
  OV_INSTPTR_fb_variable pvar;

  Ov_ForEachChild (fb_variables, pfc, pvar)
  {
    if (ov_string_compare (id, pvar->v_identifier) == 0)
    {
      return pvar;
    }
  }

  return NULL;
}

static OV_BOOL
checkSourceElement
(OV_INSTPTR_fb_localconnection pinst)
{
  OV_INSTPTR_fb_functionblock psourcefb;
  OV_INSTPTR_fb_functionchart psourcefc;
  OV_INSTPTR_ov_object psourceobj;
  OV_INSTPTR_fb_variable psourcevar, plinkedvar;
  OV_ELEMENT sourceelem;
  OV_STRING port = NULL;
  OV_TICKET dummyTicket;
  
  /* Init the ticket */

  dummyTicket.vtbl = NULL;
  dummyTicket.type = OV_TT_SIMPLE;
  dummyTicket.ticketunion.simpleticket.id = pdb->serverpassword;

  /* Get source object and source element info and test if it is a
     readable variable */

  psourceobj = NULL;

  if (ov_string_compare (pinst->v_sourceport, "") == 0)
  {
    return FALSE;
  }
  
  psourcefb =
    Ov_PtrUpCast (fb_functionblock, Ov_GetParent (fb_outputconnections, pinst));
  
  if (psourcefb)
  {
    psourcefc = Ov_DynamicPtrCast (fb_functionchart, psourcefb);
  }
  else
  {
    psourcefc = Ov_GetParent (fb_initialconnections, pinst);  
  }

  if (psourcefc)
  {
    psourcevar = getVariable (psourcefc, pinst->v_sourceport);
    if (psourcevar)
    {
      /* Link source variable */
      
      plinkedvar = Ov_GetParent (fb_sourcevariable, pinst);
      if (plinkedvar != psourcevar) 
      {
        if (plinkedvar) 
        {
          Ov_Unlink (fb_sourcevariable, plinkedvar, pinst);
          pinst->v_on = TRUE; /* Unlinking disables connection, enable it */
        }
        Ov_Link (fb_sourcevariable, psourcevar, pinst);
      }
      port = "value";
      psourceobj = Ov_PtrUpCast (ov_object, psourcevar);
    }
  }

  if (!psourceobj && psourcefb)
  {
    psourceobj = Ov_PtrUpCast (ov_object, psourcefb);
    port = pinst->v_sourceport;
  }

  if (!psourceobj) return FALSE;
  
  /* Get element */
  
  sourceelem.elemtype = OV_ET_OBJECT;
  sourceelem.pobj = psourceobj;
  
  ov_element_searchpart (&sourceelem, &pinst->v_sourceelem,
                         OV_ET_VARIABLE, port);
                  
  if (pinst->v_sourceelem.elemtype != OV_ET_VARIABLE)
  {
    return FALSE;
  }
  
  if (! (Ov_Call2 (ov_object, psourceobj, getaccess,
                   &pinst->v_sourceelem, &dummyTicket) & OV_AC_READ))
  {
    return FALSE;
  }

  /* Check if the type is supported */
  
  if (! fb_connectiontype_implemented 
      (pinst->v_sourceelem.elemunion.pvar->v_vartype)) return FALSE;
  
  return TRUE;
}

static OV_BOOL
checkTargetElement
(OV_INSTPTR_fb_localconnection pinst)
{
  OV_INSTPTR_fb_functionblock ptargetfb;
  OV_INSTPTR_fb_functionchart ptargetfc;
  OV_INSTPTR_ov_object ptargetobj;
  OV_INSTPTR_fb_variable ptargetvar, plinkedvar;
  OV_ELEMENT targetelem;
  OV_STRING port = NULL;
  OV_TICKET dummyTicket;
  
  /* Init the ticket */

  dummyTicket.vtbl = NULL;
  dummyTicket.type = OV_TT_SIMPLE;
  dummyTicket.ticketunion.simpleticket.id = pdb->serverpassword;

  /* Get target object and target element info and test if it is a
     writeable variable */

  ptargetobj = NULL;
  
  if (ov_string_compare (pinst->v_targetport, "") == 0)
  {
    return FALSE;
  }
  
  ptargetfb =
    Ov_PtrUpCast (fb_functionblock, Ov_GetParent (fb_inputconnections, pinst));
  
  if (ptargetfb)
  {
    ptargetfc = Ov_DynamicPtrCast (fb_functionchart, ptargetfb);
  }
  else
  {
    ptargetfc = Ov_GetParent (fb_finalconnections, pinst);  
  }

  if (ptargetfc)
  {
    ptargetvar = getVariable (ptargetfc, pinst->v_targetport);
    if (ptargetvar)
    {
      /* Link target variable */

      plinkedvar = Ov_GetChild (fb_targetvariable, pinst);
      if (plinkedvar != ptargetvar)
      {
        if (plinkedvar) 
        {
          Ov_Unlink (fb_targetvariable, pinst, ptargetvar);
          pinst->v_on = TRUE; /* Unlinking disables connection, enable it */
        }
        Ov_Link (fb_targetvariable, pinst, ptargetvar);
      }
      port = "value";
      ptargetobj = Ov_PtrUpCast (ov_object, ptargetvar);
    }
  }

  if ((!ptargetobj) && ptargetfb)
  {
    ptargetobj = Ov_PtrUpCast (ov_object, ptargetfb);
    port = pinst->v_targetport;
  }

  if (!ptargetobj) return FALSE;
  
  /* Get element */
  
  targetelem.elemtype = OV_ET_OBJECT;
  targetelem.pobj = ptargetobj;
  
  ov_element_searchpart (&targetelem, &pinst->v_targetelem,
                         OV_ET_VARIABLE, port);
  
  if (pinst->v_targetelem.elemtype != OV_ET_VARIABLE)
  {
    return FALSE;
  }
  
  if (! (Ov_Call2 (ov_object, ptargetobj, getaccess,
                   &pinst->v_targetelem, &dummyTicket) & OV_AC_WRITE))
  {
    return FALSE;
  }

  /* TODO: flags */

  if (IsFlagSet (pinst->v_targetelem.elemunion.pvar->v_flags, 'p'))
  {
    return FALSE;
  }

  return TRUE;
}

/* Return true if the connection has been initialized, otherwise false. */

OV_DLLFNCEXPORT OV_BOOL
fb_localconnection_checkelements
(OV_INSTPTR_fb_connection	pconn)
{
  OV_INSTPTR_fb_localconnection pinst =
    Ov_StaticPtrCast(fb_localconnection, pconn);
  
  if (! checkSourceElement (pinst)) return FALSE;
  if (! checkTargetElement (pinst)) return FALSE;
  
  /* check if source and target port have the same type and vector
     length */
  
  /* Allow implicit conversion to and from ANY (TODO: is this safe?) */

  if (pconn->v_sourceelem.elemunion.pvar->v_vartype == OV_VT_ANY ||
      pconn->v_targetelem.elemunion.pvar->v_vartype == OV_VT_ANY)
  {
    return TRUE;
  }

  if (pconn->v_sourceelem.elemunion.pvar->v_vartype
      != pconn->v_targetelem.elemunion.pvar->v_vartype)
  {
    return FALSE;
  }

  if (pconn->v_sourceelem.elemunion.pvar->v_vartype == OV_VT_STRUCT)
  {
    if(Ov_GetParent(ov_construction, pconn->v_sourceelem.elemunion.pvar)
       != Ov_GetParent(ov_construction, pconn->v_targetelem.elemunion.pvar))
    {
      return FALSE;
    }
  }
  
  if (pconn->v_sourceelem.elemunion.pvar->v_vartype == OV_VT_BYTE_VEC)
  {
    if (strcmp (pconn->v_sourceelem.elemunion.pvar->v_ctypename,
                pconn->v_targetelem.elemunion.pvar->v_ctypename))
    {
      return FALSE;
    }
  }
  
  if (pconn->v_sourceelem.elemunion.pvar->v_veclen
     != pconn->v_targetelem.elemunion.pvar->v_veclen)
  {
    return FALSE;
  }
  
  return TRUE;  
}
