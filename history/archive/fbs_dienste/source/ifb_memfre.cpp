/*****************************************************************************
*                                                                            *
*    i F B S p r o                                                           *
*    #############                                                           *
*                                                                            *
*   L T S o f t                                                              *
*   Agentur für Leittechnik Software GmbH                                    *
*   Heinrich-Hertz-Straße 10                                                 *
*   50170 Kerpen                                                             *
*   Tel      : 02273/9893-0                                                  *
*   Fax      : 02273/52526                                                   *
*   e-Mail   : ltsoft@ltsoft.de                                              *
*   Internet : http://www.ltsoft.de                                          *
*                                                                            *
******************************************************************************
*                                                                            *
*   iFBSpro - Client-Bibliothek                                              *
*   IFBSpro/Client - iFBSpro  ACPLT/KS Dienste-Schnittstelle (C++)           *
*   ==============================================================           *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   ifb_memfre.cpp                                                           *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Gibt von iFBSpro-Diensten allocierten Speicherplatz frei                 *
*                                                                            *
*****************************************************************************/

/*
*        Includes
*        --------
*/

#include "ifbslibdef.h"

/*****************************************************************************/
void memfre(Dienst_param* parms)
/*****************************************************************************/
{
    InstanceItems*         delinst;
    Variables*            delvar;
    VariableItem*        delval;

    SetInstVarItems* delsetvar;
    DelInstItems*    deldinst;

    LinksItems*            dellinks;
    Child*                    delchild;

    if( !parms ) {
        return;
    }
    
    while(parms->Instance) {
        
        delinst = parms->Instance;

        parms->Instance = delinst->next;
        
        while(delinst->Inst_var) {

            delvar = delinst->Inst_var;
            delinst->Inst_var = delvar->next;
            
            while(delvar->value) {

                delval = delvar->value;
                delvar->value = delval->next;

                free(delval);
            } /* while delvar->value */

            free(delvar);

        } /* while delinst->Inst_var */

        free(delinst);

    } /* while parms->Instance */

    while(parms->Set_Inst_Var) {

        delsetvar = parms->Set_Inst_Var;
        parms->Set_Inst_Var = delsetvar->next;
        
        while(delsetvar->Inst_var) {
        
            delvar = delsetvar->Inst_var;
            delsetvar->Inst_var = delvar->next;
            
            while(delvar->value) {

                delval = delvar->value;
                delvar->value = delval->next;
                
                free(delval);
            } /* while delvar->value */

            free(delvar);

        } /* while delsetvar->Inst_var */

        free(delsetvar);

    } /* while parms->Set_Inst_Var */


    while(parms->DelInst) {
        
        deldinst = parms->DelInst;
        parms->DelInst = deldinst->next;
        
        free(deldinst);

    } /* while parms->DelInst */


    while(parms->OldLibs) {
        
        deldinst = parms->OldLibs;
        parms->OldLibs = deldinst->next;

        free(deldinst);

    } /* while parms->OldLibs */

    while(parms->NewLibs) {
        
        deldinst = parms->NewLibs;
        parms->NewLibs = deldinst->next;

        free(deldinst);

    } /* while parms->NewLibs */


    while(parms->Links) {

        dellinks = parms->Links;
        parms->Links = dellinks->next;
        
        while(dellinks->children) {

            delchild = dellinks->children;
            dellinks->children = delchild->next;
            
            free(delchild);

        } /* while dellinks->children */

        free(dellinks);

    } /* while parms->Links */

    while(parms->UnLinks) {

        dellinks = parms->UnLinks;
        parms->UnLinks = dellinks->next;
    
        while(dellinks->children) {

            delchild = dellinks->children;
            dellinks->children = delchild->next;
            
            free(delchild);

        } /* while dellinks->children */

        free(dellinks);

    } /* while parms->UnLinks */

    return;

} /* memfree() */
