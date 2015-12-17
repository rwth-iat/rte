/*

 * $Header: /home/david/cvs/acplt/ks/include/ks/ks_xdr.x,v 1.7 2008-09-22 08:26:09 henning Exp $

 *

 * ks_xdr.x -- Specification of the ACPLT/KS protocol for use with the rpcgen

 *   protocol compiler for ONC/RPC. This ONC/RPC protocol specification is

 *   split up into two main files: this file containing the definitions of

 *   data of structures, and the ks.h file which contains the enumerations

 *   and some constants. The reason for this is that we use the enumerations

 *   and constants also in our C++ ACPLT/KS framework, while we don't use the

 *   rpcgen generated code stuff and data structures, because that is much

 *   easier to do in C++ itself with template arrays, lists, etc. which are

 *   able to "xdr" themselves into XDR streams.

 *

 * ACPLT is a registered trademark

 * of the Chair of Process Control Engineering

 *

 * (c) 1996, 1997, 1998, 1999 Chair of Process Control Engineering,

 * Aachen University of Technology, Germany

 *

 * Contact ks@plt.rwth-aachen.de for more information about ACPLT/KS, or

 * take a look at our web site: http://www.plt.rwth-aachen.de/ks/english

 *

 * Redistribution and use in source and binary forms, with or without

 * modification, are permitted provided that the following conditions

 * are met:

 * 1. Redistributions of source code must retain the above copyright

 *    notice, this list of conditions and the following disclaimer.

 * 2. Redistributions in binary form must print or display the above

 *    copyright notice either during startup or must have a means for

 *    the user to view the copyright notice.

 * 3. Redistributions in binary form must reproduce the above copyright

 *    notice, this list of conditions and the following disclaimer in the

 *    documentation and/or other materials provided with the distribution.

 * 4. Neither the name of the Chair of Process Control Engineering nor the

 *    name of the Aachen University of Technology may be used to endorse or

 *    promote products derived from this software without specific prior

 *    written permission.

 *

 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING

 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED

 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR

 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL

 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,

 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,

 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;

 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,

 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR

 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF

 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */



/*

 * Preprocessor symbols used:

 *   SEPARATE_AUTH -- define to suppress emitting code for full blown request

 *                    and reply structures, when you're encoding/decoding auth

 *                    information independent of the particular request/reply.

 *   EMIT_PROCS    -- define to get the function/service stubs.

 */



/* ----------------------------------------------------------------------------

 * Note that enumerations and constants are defined in the ks/ks.h header

 * file (which originally belongs to the C++ Communication Library) and that

 * this file is not used in the rpcgen run itself, but only included when the

 * rpcgen-generated header and source files are compiled.

 */

%#include <ks/ks.h>





/* ----------------------------------------------------------------------------

 * Because the enumerations are defined in the ks/ks.h header file, we only

 * need the xdr_xxxx functions emitted into the ks_xdr.c file, but we don't

 * want the enums to be emitted into the ks_xdr.h header file.

 */

#ifdef RPC_XDR

enum KS_RESULT              { DUMMY = 0 };

enum KS_AUTH_TYPE           { DUMMY = 0 };

enum KS_VAR_TYPE            { DUMMY = 0 };

enum KS_STATE               { DUMMY = 0 };

enum KS_OBJ_TYPE            { DUMMY = 0 };

enum KS_ACCESS              { DUMMY = 0 };

enum KS_LINK_TYPE           { DUMMY = 0 };

enum KS_HIST_TYPE           { DUMMY = 0 };

enum KS_PLACEMENT_HINT      { DUMMY = 0 };

enum KS_INTERPOLATION_MODE  { DUMMY = 0 };

enum KS_STRUCTURE_FLAGS     { DUMMY = 0 };

enum KS_EP_FLAGS            { DUMMY = 0 };



typedef u_long KS_SEMANTIC_FLAGS;

#endif





/* ----------------------------------------------------------------------------

 * Time with ACPLT/KS has a possible maximum resolution of 1 microsecond.

 * Absolute time is measured in Universal Time Coordinates (UTC). Note that

 * absolute time can't be negative.

 */

struct KS_TIME {

    u_long  secs;

    u_long  usecs;

};





/* ----------------------------------------------------------------------------

 * In addition to absolute time coordinates, ACPLT/KS also knows of time spans

 * as per protocol version 2. Note that both secs and usecs are signed and

 * that they both always must have the same sign (it's an invariant!).

 */

struct KS_TIME_SPAN {

    long    secs;

    long    usecs;

};





/* ----------------------------------------------------------------------------

 * The next typedef is necessary as rpcgen can't handle a straight vector

 * of strings, e.g. string<><>.

 */

typedef string KS_STRING<>;





/* ----------------------------------------------------------------------------

 * The authentification stuff...

 */

struct KS_AUTH_SIMPLE_REQ {

    string  id<KS_SIMPLEID_MAXLEN>;

};



union KS_AUTH_REQ switch (KS_AUTH_TYPE type) {

    case KS_AUTH_NONE:

        void;

    case KS_AUTH_SIMPLE:

        KS_AUTH_SIMPLE_REQ  simple;

};



union KS_AUTH_REPLY switch (KS_AUTH_TYPE type) {

    case KS_AUTH_NONE:

        void;

    case KS_AUTH_SIMPLE:

        void;

    case 0xDeadBeef:

        u_long DeadBeef;

};





/* ----------------------------------------------------------------------------

 * Available value data types, and all the engineered properties stuff for

 * variables.

 */

union KS_VAR_VALUE switch (KS_VAR_TYPE type) {

    case KS_VT_VOID:           void;

    case KS_VT_BOOL:           bool          val_bool;

    case KS_VT_INT:            long          val_int;

    case KS_VT_UINT:           u_long        val_uint;

    case KS_VT_SINGLE:         float         val_single;

    case KS_VT_DOUBLE:         double        val_double;

    case KS_VT_STRING:         KS_STRING     val_string;

    case KS_VT_TIME:           KS_TIME       val_time;

    case KS_VT_TIME_SPAN:      KS_TIME_SPAN  val_time_span;

    case KS_VT_BYTE_VEC:       opaque        val_opaque<>;

    case KS_VT_BOOL_VEC:       bool          val_bool_vec<>;

    case KS_VT_INT_VEC:        long          val_int_vec<>;

    case KS_VT_UINT_VEC:       u_long        val_uint_vec<>;

    case KS_VT_SINGLE_VEC:     float         val_single_vec<>;

    case KS_VT_DOUBLE_VEC:     double        val_double_vec<>;

    case KS_VT_STRING_VEC:     KS_STRING     val_string_vec<>;

    case KS_VT_TIME_VEC:       KS_TIME       val_time_vec<>;

    case KS_VT_TIME_SPAN_VEC:  KS_TIME_SPAN  val_time_span_vec<>;

};



struct KS_VAR_ENGINEERED_PROPS {

    string       tech_unit<KS_TECHUNIT_MAXLEN>;

    KS_VAR_TYPE  type;

};



struct KS_VAR_CURRENT_PROPS {

    KS_VAR_VALUE  value;

    KS_TIME       time;

    KS_STATE      state;

};





/* ----------------------------------------------------------------------------

 * The engineered properties stuff for domains.

 */

struct KS_DOMAIN_ENGINEERED_PROPS {

    string  class_identifier<>;

};





/* ----------------------------------------------------------------------------

 * The engineered properties stuff for links.

 */

struct KS_LINK_ENGINEERED_PROPS {

    KS_LINK_TYPE  type;

    string        opposite_role_identifier<>;

    string        association_identifier<>;

};





/* ----------------------------------------------------------------------------

 * The engineered properties stuff for domains.

 */

struct KS_STRUCTURE_ENGINEERED_PROPS {

    KS_STRUCTURE_FLAGS  structure_flags;

};





/* ----------------------------------------------------------------------------

 * The varying part of the engineered properties.

 */

union KS_OBJ_VARIANT_ENGINEERED_PROPS switch (KS_OBJ_TYPE type) {

    case KS_OT_DOMAIN:

        KS_DOMAIN_PROJECTED_PROPS    domain_engineered_props;

    case KS_OT_VARIABLE:

        KS_VAR_PROJECTED_PROPS       var_engineered_props;

    case KS_OT_HISTORY:

        void; /* FIXME */

    case KS_OT_LINK:

        KS_LINK_PROJECTED_PROPS      link_engineered_props;

    case KS_OT_STRUCTURE:

	void; /* FIXME */

/*        KS_STRUCTURE_PROJECTED_PROPS structure_engineered_props; */

    default:

        void;

};





/* ----------------------------------------------------------------------------

 * This is THE structure that contains all ENGINEERED properties within

 * a communication object of ANY type. The variant part must come first,

 * as the type code also indicates what (overall) type of proj. properties

 * will be waiting for us as we pull it off the wire.

 */

struct KS_OBJ_ENGINEERED_PROPS {

    KS_OBJ_VARIANT_ENGINEERED_PROPS  variant;

    string                           identifier<KS_NAME_MAXLEN>;

    KS_TIME                          creation_time;

    string                           comment<KS_COMMENT_MAXLEN>;

    KS_ACCESS                        access_mode;

    KS_SEMANTIC_FLAGS                semantic_flags;

};





union KS_OBJ_VARIANT_CURRENT_PROPS switch (KS_OBJ_TYPE type) {

    case KS_OT_DOMAIN:

        void;

    case KS_OT_VARIABLE:

        KS_VAR_CURRENT_PROPS  var_current_props;

    case KS_OT_HISTORY:

        void; /* FIXME */

    case KS_OT_LINK:

        void; /* FIXME */

    case KS_OT_STRUCTURE:

        void; /* FIXME */

    default:

        void;

};





/* ----------------------------------------------------------------------------

 * This is THE structure that contains all CURRENT properties within

 * a communication object of ANY type.

 */

struct KS_OBJ_CURRENT_PROPS {

    KS_OBJ_VARIANT_CURRENT_PROPS  variant;

};





/* ----------------------------------------------------------------------------

 * Data structures concerning server management and the client/server connect

 * phase. This is the server/manager service group.

 */

struct KS_SERVERDESC {

    string   name<KS_NAME_MAXLEN>;

    u_short  protocol_version;

};





/* ----------------------------------------------------------------------------

 * Register a KS server with a KS manager.

 */

struct KS_REGISTRATION_PAR { /* The service's parameters */

    KS_SERVERDESC  server;

    u_short        port;

    u_long         time_to_live;

};



#ifndef SEPARATE_AUTH

struct KS_REGISTRATION_REQ { /* This is the full request packet */

    KS_AUTH_REQ          auth;

    KS_REGISTRATION_PAR  par;

};

#endif



struct KS_REGISTRATION_RET {

    KS_RESULT  result;

};



#ifndef SEPARATE_AUTH

struct KS_REGISTRATION_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY        auth;

    KS_REGISTRATION_RET  ret;

};

#endif





/* ----------------------------------------------------------------------------

 * Unregister a KS server

 */

struct KS_UNREGISTRATION_PAR { /* The service's parameters */

    KS_SERVERDESC  server;

};



#ifndef SEPARATE_AUTH

struct KS_UNREGISTRATION_REQ { /* This is the full request packet */

    KS_AUTH_REQ            auth;

    KS_UNREGISTRATION_PAR  par;

};

#endif



struct KS_UNREGISTRATION_RET {

    KS_RESULT  result;

};



#ifndef SEPARATE_AUTH

struct KS_UNREGISTRATION_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY          auth;

    KS_UNREGISTRATION_RET  ret;

};

#endif





/* ----------------------------------------------------------------------------

 * Lookup a KS server

 */

struct KS_GETSERVER_PAR { /* The service's parameters */

    KS_SERVERDESC  server;

};



#ifndef SEPARATE_AUTH

struct KS_GETSERVER_REQ { /* This is the full request packet */

    KS_AUTH_REQ       auth;

    KS_GETSERVER_PAR  par;

};

#endif



struct KS_SERVER_STATE {

    KS_SERVERDESC  server;

    u_short        port;

    KS_TIME        expires_at;

    bool           living;

};



union KS_GETSERVER_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        KS_SERVER_STATE  server_state;

    default:

        void;

};



#ifndef SEPARATE_AUTH

struct KS_GETSERVER_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY     auth;

    KS_GETSERVER_RET  ret;

};

#endif





/* ----------------------------------------------------------------------------

 * Service group for walking through the object tree.

 */

struct KS_GETEP_PAR { /* The service's parameters */

    string       path<>;

    KS_OBJ_TYPE  type_mask;

    string       name_mask<>;

    KS_EP_FLAGS  scope_flags;

};



#ifndef SEPARATE_AUTH

struct KS_GETEP_REQ { /* This is the full request packet */

    KS_AUTH_REQ   auth;

    KS_GETEP_PAR  par;

};

#endif



union KS_GETEP_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        KS_OBJ_ENGINEERED_PROPS  items<>;

    default:

        void;

};



#ifndef SEPARATE_AUTH

struct KS_GETEP_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY  auth;

    KS_GETEP_RET   ret;

};

#endif





/* -----------------------------------------------------------------------------

 * All the data structures necessary for the variable access service group.

 */

union KS_GETVAR_ITEM switch (KS_RESULT result) {

    case KS_ERR_OK:

        KS_OBJ_CURRENT_PROPS  item;

    default:

        void;

};



struct KS_GETVAR_PAR { /* The service's parameters */

    KS_STRING    identifiers<>;

};



#ifndef SEPARATE_AUTH

struct KS_GETVAR_REQ { /* This is the full request packet */

    KS_AUTH_REQ    auth;

    KS_GETVAR_PAR  par;

};

#endif



union KS_GETVAR_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        KS_GETVAR_ITEM  items<>;

    default:

        void;

};



#ifndef SEPARATE_AUTH

struct KS_GETVAR_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY  auth;

    KS_GETVAR_RET  ret;

};

#endif





struct KS_SETVAR_ITEM {

    string                path_and_name<>;

    KS_OBJ_CURRENT_PROPS  current_props;

};





struct KS_SETVAR_PAR { /* The service's parameters */

    KS_SETVAR_ITEM  items<>;

};



#ifndef SEPARATE_AUTH

struct KS_SETVAR_REQ { /* This is the full request packet */

    KS_AUTH_REQ    auth;

    KS_SETVAR_PAR  par;

};

#endif



union KS_SETVAR_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        KS_RESULT  results<>;

    default:

        void;

};



#ifndef SEPARATE_AUTH

struct KS_SETVAR_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY  auth;

    KS_SETVAR_RET  ret;

};

#endif





struct KS_EXGDATA_PAR { /* The service's parameters */

    KS_SETVAR_ITEM  set_vars<>;

    KS_STRING       get_vars<>;

};



#ifndef SEPARATE_AUTH

struct KS_EXGDATA_REQ { /* This is the full request packet */

    KS_AUTH_REQ     auth;

    KS_EXGDATA_PAR  par;

};

#endif



struct KS_EXGDATA_ITEMS {

    KS_RESULT       results<>;

    KS_GETVAR_ITEM  items<>;

};



union KS_EXGDATA_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        KS_EXGDATA_ITEMS  items;

    default:

        void;

};



#ifndef SEPARATE_AUTH

struct KS_EXGDATA_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY   auth;

    KS_EXGDATA_RET  ret;

};

#endif





/* -----------------------------------------------------------------------------

 * All the data structures necessary for the object management service group.

 */

union KS_PLACEMENT switch (KS_PLACEMENT_HINT hint) {

    case KS_PMH_BEFORE:

    case KS_PMH_AFTER:

        string place_path<>;

    default:

        void;

};



struct KS_LINK_ITEM {

    string        link_path<>;

    string        element_path<>;

    KS_PLACEMENT  place;

    KS_PLACEMENT  opposite_place;

};



struct KS_UNLINK_ITEM {

    string  link_path<>;

    string  element_path<>;

};





/* -----------------------------------------------------------------------------

 * The dazzling CreateObject service. Whow. Sigh.

 */

struct KS_CREATEOBJ_ITEM {

    string          factory_path<>;

    string          new_path<>;

    KS_PLACEMENT    place;

    KS_SETVAR_ITEM  parameters<>;

    KS_LINK_ITEM    links<>;

};



struct KS_CREATEOBJECT_PAR {

    KS_CREATEOBJ_ITEM  items<>;

};



#ifndef SEPARATE_AUTH

struct KS_CREATEOBJECT_REQ { /* This is the full request packet */

    KS_AUTH_REQ          auth;

    KS_CREATEOBJECT_PAR  par;

};

#endif



struct KS_CREATEOBJECTPAR_RESULTS {

    KS_RESULT  params_results<>;

    KS_RESULT  link_results<>;

};



union KS_CREATEOBJECTITEM_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        void; /* !!!! */

    case KS_ERR_BADINITPARAM:

        KS_CREATEOBJECTPAR_RESULTS  results;

    default:

        void;

};



union KS_CREATEOBJECT_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

	KS_CREATEOBJECTITEM_RET  obj_results<>;

    default:

        void;

};



#ifndef SEPARATE_AUTH

struct KS_CREATEOBJECT_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY        auth;

    KS_CREATEOBJECT_RET  ret;

};

#endif





/* -----------------------------------------------------------------------------

 * The DeleteObject service.

 */

struct KS_DELETEOBJECT_PAR {

    KS_STRING  paths<>;

};



#ifndef SEPARATE_AUTH

struct KS_DELETEOBJECT_REQ { /* This is the full request packet */

    KS_AUTH_REQ          auth;

    KS_DELETEOBJECT_PAR  par;

};

#endif



union KS_DELETEOBJECT_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        KS_RESULT  results<>;

    default:

        void;

};



#ifndef SEPARATE_AUTH

struct KS_DELETEOBECT_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY        auth;

    KS_DELETEOBJECT_RET  ret;

};

#endif





/* -----------------------------------------------------------------------------

 * The RenameObject service.

 */

struct KS_RENAMEOBJECT_ITEM {

    string        old_path<>;

    string        new_path<>;

    KS_PLACEMENT  place;

};



struct KS_RENAMEOBJECT_PAR {

    KS_RENAMEOBJECT_ITEM  items<>;

};



#ifndef SEPARATE_AUTH

struct KS_RENAMEOBJECT_REQ { /* This is the full request packet */

    KS_AUTH_REQ          auth;

    KS_RENAMEOBJECT_PAR  par;

};

#endif



union KS_RENAMEOBJECT_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        KS_RESULT  results<>;

    default:

        void;

};



#ifndef SEPARATE_AUTH

struct KS_RENAMEOBJECT_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY        auth;

    KS_RENAMEOBJECT_RET  ret;

};

#endif







/* -----------------------------------------------------------------------------

 * The GetCanonicalPath service.

 */

struct KS_GETCANONICALPATH_PAR {

    KS_STRING  paths<>;

};



#ifndef SEPARATE_AUTH

struct KS_UNLINK_REQ { /* This is the full request packet */

    KS_AUTH_REQ              auth;

    KS_GETCANONICALPATH_PAR  par;

};

#endif



union KS_GETCANONICALPATHITEM_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        string  canonical_path<>;

    default:

        void;

};



union KS_GETCANONICALPATH_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        KS_GETCANONICALPATHITEM_RET  results<>;

    default:

        void;

};



#ifndef SEPARATE_AUTH

struct KS_GETCANONICALPATH_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY            auth;

    KS_GETCANONICALPATH_RET  ret;

};

#endif





/* -----------------------------------------------------------------------------

 * The Link service.

 */

struct KS_LINK_PAR {

    KS_LINK_ITEM  items<>;

};



#ifndef SEPARATE_AUTH

struct KS_LINK_REQ { /* This is the full request packet */

    KS_AUTH_REQ  auth;

    KS_LINK_PAR  par;

};

#endif



union KS_LINK_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        KS_RESULT  results<>;

    default:

        void;

};



#ifndef SEPARATE_AUTH

struct KS_LINK_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY  auth;

    KS_LINK_RET    ret;

};

#endif





/* -----------------------------------------------------------------------------

 * The Unlink service.

 */

struct KS_UNLINK_PAR {

    KS_UNLINK_ITEM  items<>;

};



#ifndef SEPARATE_AUTH

struct KS_UNLINK_REQ { /* This is the full request packet */

    KS_AUTH_REQ    auth;

    KS_UNLINK_PAR  par;

};

#endif



union KS_UNLINK_RET switch (KS_RESULT result) {

    case KS_ERR_OK:

        KS_RESULT  results<>;

    default:

        void;

};



#ifndef SEPARATE_AUTH

struct KS_UNLINK_REPLY { /* This is the full reply packet */

    KS_AUTH_REPLY  auth;

    KS_UNLINK_RET  ret;

};

#endif





#ifdef EMIT_PROCS

/* ----------------------------------------------------------------------------

 * Finally, the procedures -- aka exported services -- provided by KS servers

 * and KS managers.

 *

 * Note that this part is only used as a description, as we don't use the

 * server and client stubs which could be generated by rpcgen. Thus, this part

 * is #ifdef'd out.

 */

program KS_PROG {

version KS_VERS_CURRENT {

    /*

     * The unavoidable NULL procedure. It's just there for the purpose of

     * pinging (test the health status of a server).

     */

    void KS_NULL(void) = 0;

    

    /*

     * Now for the manager part of the KS protocol

     */

    KS_REGISTRATION_REPLY KS_REGISTER(KS_REGISTRATION_REQ)

        = 0x0000FF01;

    KS_UNREGISTRATION_REPLY KS_UNREGISTER(KS_UNREGISTRATION_REQ)

        = 0x0000FF02;

    KS_GETSERVER_REPLY KS_GETSERVER(KS_GETSERVER_REQ)

        = 0x0000FF03;

    

    /*

     * The GetOD services group.

     */

    KS_GETEP_REPLY KS_GETEP(KS_GETEP_REQ)

        = 0x00000002;

    

    /*

     * The variable access service group.

     */

    KS_GETVAR_REPLY KS_GETVAR(KS_GETVAR_REQ)

        = 0x00000101;

    KS_SETVAR_REPLY KS_SETVAR(KS_SETVAR_REQ)

        = 0x00000102;

    KS_EXGDATA_REPLY KS_EXGDATA(KS_EXGDATA_REQ)

        = 0x00000103;

    

} = 2;       /* end of version level */

} = 0x49678; /* Basic KS protocol */



const KS_VERS = 2; /* this is the latest protocol revision */

#endif





/* End of ks_xdr.x */

