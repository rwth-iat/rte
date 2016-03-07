
#ifndef _FB_DIENSTPARAMS_H_
#define _FB_DIENSTPARAMS_H_

#include "plt/list.h"
#include "ks/ks.h"

#ifdef __cplusplus
extern "C" {
#endif

//  ----------------------------------------------------------------------------
//  Object types. 
//  ----------------------------------------------------------------------------

ENUMDEF(FB_OBJ_TYPE)

#define FB_OBJ_VARIABLE   ENUMVAL(FB_OBJ_TYPE, 0x0001) /* object is a variable  */
#define FB_OBJ_LINK       ENUMVAL(FB_OBJ_TYPE, 0x0002) /* object is a link      */
#define FB_OBJ_TASK       ENUMVAL(FB_OBJ_TYPE, 0x0004) /* object is a task      */
#define FB_OBJ_LIBRARY    ENUMVAL(FB_OBJ_TYPE, 0x0008) /* object is a library   */
#define FB_OBJ_CLASS      ENUMVAL(FB_OBJ_TYPE, 0x0010) /* object is a class     */
#define FB_OBJ_CONNECTION ENUMVAL(FB_OBJ_TYPE, 0x0020) /* object is a connection */
#define FB_OBJ_INSTANCE   ENUMVAL(FB_OBJ_TYPE, 0x0040) /* object is a instance of user defined class */
#define FB_OBJ_CONTAINER  ENUMVAL(FB_OBJ_TYPE, 0x0080) /* object is a container */
#define FB_OBJ_HISTORY    ENUMVAL(FB_OBJ_TYPE, 0x0100) /* object is a history   */
#define FB_OBJ_ANY        ENUMVAL(FB_OBJ_TYPE, \
                                     FB_OBJ_VARIABLE | FB_OBJ_LINK | \
                                     FB_OBJ_TASK     | FB_OBJ_LIBRARY | \
                                     FB_OBJ_CLASS    | FB_OBJ_CONNECTION | \
                                     FB_OBJ_INSTANCE | FB_OBJ_CONTAINER  | \
                                     FB_OBJ_HISTORY)


#ifdef __cplusplus
}; /* extern "C" */
#endif

///////////////////////////////////////////////////////////////////////////////
//  Object properties
///////////////////////////////////////////////////////////////////////////////

class ObjProps {
public :
    ObjProps() {}
    ObjProps(const ObjProps &orig);
    ~ObjProps() {}

	// Operatoren
	bool operator == (const ObjProps &rhs) const
						{ return (identifier == rhs.identifier); }

	ObjProps & operator = (ObjProps &rhs)  {
            identifier = rhs.identifier;
            creation_time = rhs.creation_time;
            comment = rhs.comment;
            access = rhs.access;
            flags = rhs.flags;
            obj_type = rhs.obj_type;
            class_ident = rhs.class_ident;
            tech_unit = rhs.tech_unit;
            var_type = rhs.var_type;
            value = rhs.value;
            state = rhs.state;
            len = rhs.len;
			return *this; }

    // Variables

    PltString          identifier;     // identifier of object
    KsTime             creation_time;  // creation timestamp
    PltString          comment;        // some description about object
    KS_ACCESS          access;         // access mode and other mode flags
    KS_SEMANTIC_FLAGS  flags;          // semantic flags for object
    FB_OBJ_TYPE        obj_type;       // object type (FB_OBJ_INSTANCE or FB_OBJ_VARIABLE or ...)
    PltString          class_ident;    // class identifier
    PltString          tech_unit;      // units
    KS_VAR_TYPE        var_type;       // variable type (KS_VT_INT ...)
    PltString          value;          // value, if object is a variable or link
    KS_STATE           state;          // state, if object is a variable
    size_t             len;            // length, if variable is a vector
};

///////////////////////////////////////////////////////////////////////////////
//  Connection properties
///////////////////////////////////////////////////////////////////////////////

class ConData {
public :
    ConData() {}
    ConData(const ConData &orig);
    ~ConData() {}
	// Operatoren
	bool operator == (const ConData &rhs) const
						{ return (identifier == rhs.identifier); }
    ConData & operator = (ConData &rhs)  {
            identifier = rhs.identifier;
            source_fb = rhs.source_fb;
            source_port = rhs.source_port;
            target_fb = rhs.target_fb;
            target_port = rhs.target_port;
            on = rhs.on;
            source_trig = rhs.source_trig; {}
    		return *this; }

    // Variables

    PltString          identifier;     // identifier of object
    PltString          conclas;
    PltString          source_fb;
    PltString          source_port;
    PltString          target_fb;
    PltString          target_port;
    bool               on;             // true, if connection is on
    bool               source_trig;    // connection mode (get/send)
};

///////////////////////////////////////////////////////////////////////////////
//  Parameter for function IFBS_DIR, IFBS_GETCONS
///////////////////////////////////////////////////////////////////////////////

class FbDirParams {
public :
    FbDirParams() {
        scope_flags = KS_EPF_DEFAULT;
    }
    FbDirParams(FbDirParams &orig);
    ~FbDirParams() {}

	FbDirParams & operator = (FbDirParams &rhs) {
		  path = rhs.path;
		  type_mask   = rhs.type_mask;
		  name_mask   = rhs.name_mask;
		  scope_flags = rhs.scope_flags;
		  return *this;
    }

    // Variables
    PltString    path;
    FB_OBJ_TYPE  type_mask;
    PltString    name_mask;
    KS_EP_FLAGS  scope_flags;
};


///////////////////////////////////////////////////////////////////////////////
//  Parameter for function IFBS_CREATE_INST
///////////////////////////////////////////////////////////////////////////////

class FbCreateInstParams {
public :
    FbCreateInstParams() {}
    FbCreateInstParams(FbCreateInstParams &orig);
    ~FbCreateInstParams() {}
	FbCreateInstParams & operator = (FbCreateInstParams &rhs) {
		  path = rhs.path;
          factory = rhs.factory;
		  return *this; }

    // Variables
    PltString    path;
    PltString    factory;
};


///////////////////////////////////////////////////////////////////////////////
//  Parameter for function IFBS_LINK_TO_TASK
///////////////////////////////////////////////////////////////////////////////

class FbLinkParams {
public :
    FbLinkParams() {}
    FbLinkParams(FbLinkParams &orig);
    ~FbLinkParams() {}
	FbLinkParams & operator = (FbLinkParams &rhs) {
		  parent_path = rhs.parent_path;
          elem_path = rhs.elem_path;
          place = rhs.place;
          place_path = rhs.place_path;
		  return *this; }

    // Variables
    PltString         parent_path;
    PltString         elem_path;
    KS_PLACEMENT_HINT place;
    PltString         place_path;
};

///////////////////////////////////////////////////////////////////////////////
//  FbSetVar-Parameter
///////////////////////////////////////////////////////////////////////////////

class FbSetVarParams {
public :
    FbSetVarParams();
    FbSetVarParams(FbSetVarParams &orig);
    ~FbSetVarParams() {}
	FbSetVarParams & operator = (FbSetVarParams &rhs) {
		  path = rhs.path;
          value = rhs.value;
          state = rhs.state;
		  return *this; }

    // Variables
    PltString         path;
    PltString         value;
    KS_STATE          state;
};

///////////////////////////////////////////////////////////////////////////////
//  Association-Parameter
///////////////////////////////////////////////////////////////////////////////

class FbAssoParam {
public :
    FbAssoParam();
    FbAssoParam(FbAssoParam &orig);
    ~FbAssoParam() {}
	FbAssoParam & operator = (FbAssoParam &rhs) {
		  identifier = rhs.identifier;
          parent_ident = rhs.parent_ident;
          parent_class = rhs.parent_class;
          parent_path  = rhs.parent_path;
          child_ident = rhs.child_ident;
          child_class = rhs.child_class;
          child_path  = rhs.child_path;
		  return *this; }

    // Variables
    PltString         identifier;
    PltString         parent_ident;
    PltString         parent_class;
    PltString         parent_path;
    PltString         child_ident;
    PltString         child_class;
    PltString         child_path;
};

///////////////////////////////////////////////////////////////////////////////
//   Inline implementation
///////////////////////////////////////////////////////////////////////////////

inline
ObjProps::ObjProps(const ObjProps &orig):
    identifier(orig.identifier),
    creation_time(orig.creation_time),
    comment(orig.comment),
    access(orig.access),
    flags(orig.flags),
    obj_type(orig.obj_type),
    class_ident(orig.class_ident),
    tech_unit(orig.tech_unit),
    var_type(orig.var_type),
    value(orig.value),
    state(orig.state),
    len(orig.len) {}


inline
ConData::ConData(const ConData &orig):
    identifier(orig.identifier),
    source_fb(orig.source_fb),
    source_port(orig.source_port),
    target_fb(orig.target_fb),
    target_port(orig.target_port),
    on(orig.on),
    source_trig(orig.source_trig) {}


inline
FbDirParams::FbDirParams(FbDirParams &orig):
    path(orig.path),
    type_mask(orig.type_mask),
    name_mask(orig.name_mask),
    scope_flags(orig.scope_flags) {}

inline
FbCreateInstParams::FbCreateInstParams(FbCreateInstParams &orig):
    path(orig.path),
    factory(orig.factory) {}

inline
FbLinkParams::FbLinkParams(FbLinkParams &orig):
    parent_path(orig.parent_path),
    elem_path(orig.elem_path),
    place(orig.place),
    place_path(orig.place_path) {}

inline
FbSetVarParams::FbSetVarParams():
    path(""),
    value(""),
    state(0) {}

inline
FbSetVarParams::FbSetVarParams(FbSetVarParams &orig):
    path(orig.path),
    value(orig.value),
    state(orig.state) {}

inline
FbAssoParam::FbAssoParam():
    identifier(""),
    parent_ident(""),
    parent_class(""),
    parent_path(""),
    child_ident(""),
    child_class(""),
    child_path("") {}
    
inline
FbAssoParam::FbAssoParam(FbAssoParam &orig):
    identifier(identifier),
    parent_ident(orig.parent_ident),
    parent_class(orig.parent_class),
    parent_path(orig.parent_path),
    child_ident(orig.child_ident),
    child_class(orig.child_class),
    child_path(orig.child_path) {}
    

#endif
