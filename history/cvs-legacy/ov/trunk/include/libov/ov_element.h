/*
*   $Id: ov_element.h,v 1.1 1999-07-19 15:02:03 dirk Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	08-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_ELEMENT_H_INCLUDED
#define OV_ELEMENT_H_INCLUDED

#include "libov/ov_ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	OV_ELEM_TYPE:
*	-------------
*	An element represents a part of the instance model in the
*	database. Elements are polymorphic, they can either be objects,
*	variables, members of structures, operations, head-links or
*	anchor links. OV_ELEM_TYPE indicates this element type. In
*	search masks the element types can be aggregated by ORing
*	the bit masks of different element types.
*/
enum OV_ELEM_TYPE_ENUM  {
	OV_ET_NONE		= 0x00,		/* invalid element */
	OV_ET_OBJECT	= 0x01,
	OV_ET_VARIABLE	= 0x02,
	OV_ET_MEMBER	= 0x04,
	OV_ET_HEAD		= 0x08,
	OV_ET_ANCHOR	= 0x10,
	OV_ET_OPERATION	= 0x20,
	OV_ET_ANY		= 0x3F		/* used for search masks only */
};
typedef enum_t OV_ELEM_TYPE;

/*
*	OV_ELEMENT:
*	-----------
*	An element represents a part of the instance model in the
*	database. Elements are polymorphic which is expressed by the
*	type of the element. Each element is mapped to an ACPLT/KS
*	communication object representing the element in the network.
*/
typedef struct {
	OV_ELEM_TYPE				elemtype;	/* the type of the element */
	struct OV_INST_ov_object	*pobj;		/* object this element belongs to */
	/* in case element is a variable or member of a variable: */
	OV_BYTE						*pvalue;	/* pointer to variable value */
	/* in different cases: */
	union {
		/* generic definition object pointer */
		struct OV_INST_ov_object		*pobj;
		/* in case element is a variable or member of a variable: */
		struct OV_INST_ov_variable		*pvar;
		/* in case element is a part object: */
		struct OV_INST_ov_part			*ppart;
		/* in case element is a head-link or an anchor-link: */
		struct OV_INST_ov_association	*passoc;
		/* in case element is an operation: */
		struct OV_INST_ov_operation		*pop;
		/* in case element is a class: */
		struct OV_INST_ov_class			*pclass;
	}	elemunion;
}	OV_ELEMENT;

/*
*	Search child of an element ("parent/child" in a path)
*/
OV_RESULT OV_DLLFNCEXPORT ov_element_searchchild(
	const OV_ELEMENT	*pparent,
	OV_ELEMENT			*pchild,
	OV_STRING			identifier
);

/*
*	Search part of an element ("parent.part" in a path)
*/
OV_RESULT OV_DLLFNCEXPORT ov_element_searchpart(
	const OV_ELEMENT	*pparent,
	OV_ELEMENT			*ppart,
	OV_ELEM_TYPE		mask,
	OV_STRING			identifier
);

/*
*	Get next child of an element ("parent/child" in a path)
*/
OV_RESULT OV_DLLFNCEXPORT ov_element_getnextchild(
	const OV_ELEMENT	*pparent,
	OV_ELEMENT			*pchild
);

/*
*	Get next part of an element ("parent.part" in a path)
*/
OV_RESULT OV_DLLFNCEXPORT ov_element_getnextpart(
	const OV_ELEMENT	*pparent,
	OV_ELEMENT			*ppart,
	OV_ELEM_TYPE		mask
);

/*
*	Search part of an object (subroutine)
*/
OV_RESULT ov_element_searchpart_object(
	struct OV_INST_ov_object	*pobj,
	OV_ELEMENT					*ppart,
	OV_ELEM_TYPE				mask,
	OV_STRING					identifier
);

/*
*	Search part of a variable or variable member (subroutine)
*/
OV_RESULT ov_element_searchpart_variable(
	struct OV_INST_ov_object	*pobj,
	struct OV_INST_ov_variable	*pvar,
	OV_BYTE						*pvalue,
	OV_ELEMENT					*ppart,
	OV_ELEM_TYPE				mask,
	OV_STRING					identifier
);

/*
*	Get next part of an object (subroutine)
*/
OV_RESULT ov_element_getnextpart_object(
	struct OV_INST_ov_object	*pobj,
	OV_ELEMENT					*ppart,
	OV_ELEM_TYPE				mask
);

/*
*	Get next part of a variable or variable member (subroutine)
*/
OV_RESULT ov_element_getnextpart_variable(
	struct OV_INST_ov_object	*pobj,
	struct OV_INST_ov_variable	*pvar,
	OV_BYTE						*pvalue,
	OV_ELEMENT					*ppart,
	OV_ELEM_TYPE				mask
);

/*
*	Get the identifier of an element
*/
OV_STRING OV_DLLFNCEXPORT ov_element_getidentifier(
	const OV_ELEMENT			*pelem
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#include "ov.h"

#endif
/*
*	End of file
*/

