/*****************************************************************************
*                                                                            *
*   i F B S p r o                                                            *
*   #############                                                            *
*                                                                            *
*   © L T S o f t                                                            *
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
*   blockparam.h                                                             *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Definitionen fuer iFBSpro-Dienste                                        *
*                                                                            *
*****************************************************************************/


#ifndef _BLOCKPARAM_H_
#define _BLOCKPARAM_H_

#include "plt/config.h"

#if PLT_USE_DEPRECIATED_HEADER
#include <iostream.h>
#else
#include <iostream>
#endif

#include <ks/string.h>
#include <ks/props.h>

class InstVarProps
{
public:
		// Konstruktoren
		InstVarProps() {}
		InstVarProps(KsVarCurrProps cp, KsEngPropsHandle pp);
		InstVarProps(const InstVarProps &orig);

		// Operatoren
		bool operator == (const InstVarProps &rhs) const
						{ return (eng_props == rhs.eng_props); }

		InstVarProps & operator = (InstVarProps &rhs) {
			  curr_props = rhs.curr_props;
			  eng_props = rhs.eng_props;
			  return *this; }

		// Propertis
		KsVarCurrProps 		curr_props;
		KsEngPropsHandle	eng_props;
};


inline
InstVarProps::InstVarProps(KsVarCurrProps cp, KsEngPropsHandle pp):
curr_props(cp),eng_props(pp) {}

inline
InstVarProps::InstVarProps(const InstVarProps &orig):
curr_props(orig.curr_props), eng_props(orig.eng_props) {}

#endif
