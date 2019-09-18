/*
*   Copyright (C) 2018
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/
/***********************************************************************
*
*	File:
*	------
*	cshmimodel.d.ts
*
*	Autoren:
*	--------
*	Je							Holger Jeromin <Holger.Jeromin@plt.rwth-aachen.de>
*
***********************************************************************/


// Type definitions for acplt/cshmi blackbox jsOnload and jsOnglobalvarchanged Javascript code
// Project: https://github.com/acplt/rte/tree/master/addonlibs/hmi/cshmi
// Definitions by: Holger Jeromin <https://github.com/HolgerJeromin>

/**
    JavaScript API for interaction with the CsHmi model
*/
declare namespace cshmimodel {
    /** Common callback definition for KS Communication */
    interface IKsCallback {
        (
            /** HMIJavaScriptKSClient object */
            client: object, 
            /** the plain request object */
            req: XMLHttpRequest
        ): void
    }
    interface Dictionary<T> {
        [index: string]: T | undefined;
    }

    /** html body node of the HTML content */
    let HtmlBody: HTMLBodyElement | null;
    /** SVG Element of the blackbox */
    let SvgElement: SVGElement;
    /** first HTML element of blackbox html content */
    let HtmlFirstElement: HTMLElement | null;
    let Modelpath: string;
    /** document object of the blackbox */
    let document: Document;
    /** Window object of the blackbox */
    let window: Window;
    /** API to variables below the blackbox object */
    let variables: Dictionary<{
        varName: string;
        getValue: () => string;
        setValue: (newValue: string) => void;
    }>;
    /**
     * Creates a new template below the current blackbox
     * @param x
     * @param y
     * @param rotate
     * @param hideable 
     * @param PathOfTemplateDefinition 
     * @param FBReference 
     * @param FBVariableReference 
     * @param ConfigValues 
     */
    function instantiateTemplate (
        x: string,
        y: string,
        rotate: string,
        hideable: string,
        PathOfTemplateDefinition: string,
        FBReference: string,
        FBVariableReference: string,
        ConfigValues: string
    ): void;
    /**
     * Requests an Engineering Property
     * @param path of object to query
     * @param requestType = OT_DOMAIN type of KS Object to query ("OT_DOMAIN", "OT_VARIABLE", "OT_LINK" or "OT_ANY"). "OT_DOMAIN" if not supplied
     * @param requestOutput Array of interesting objects properties ("OP_NAME", "OP_TYPE", "OP_COMMENT", "OP_ACCESS", "OP_SEMANTIC", "OP_CREATIONTIME", "OP_CLASS" or "OT_ANY"). "OP_NAME" if not supplied
     * @param cbfnc callback function for a async request
     * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used). "text/tcl" if not supplied
     * @return "{fb_hmi1} {fb_hmi2} {fb_hmi3} {MANAGER} {fb_hmi4} {fb_hmi5}" or null or true (if callback used)
     */
    function getEP (
        path: string,
        requestType?: "OT_DOMAIN" | "OT_VARIABLE" | "OT_LINK" | "OT_ANY",
        requestOutput?: "OP_NAME" | "OP_TYPE" | "OP_COMMENT" | "OP_ACCESS" | "OP_SEMANTIC" | "OP_CREATIONTIME" | "OP_CLASS" | ("OP_NAME" | "OP_TYPE" | "OP_COMMENT" | "OP_ACCESS" | "OP_SEMANTIC" | "OP_CREATIONTIME" | "OP_CLASS")[] | "OT_ANY",
        cbfnc?: IKsCallback,
        responseFormat?: "text/tcl" | "text/ksx" | "text/plain"
    ): string | null | true;
    /**
     * Requests a KS Variable
     * @param path of the variable to fetch, multiple path possible via an Array
     * @param requestOutput Array of interesting objects properties ("OP_NAME", "OP_TYPE", "OP_VALUE", "OP_TIMESTAMP" or "OP_STATE"). "OP_VALUE" if not supplied
     * @param cbfnc callback function for a async request
     * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used). "text/tcl" if not supplied
     * @return "{{/TechUnits/HMIManager}}", response: "{/TechUnits/Sheet1}" or "TksS-0042::KS_ERR_BADPATH {{/Libraries/hmi/Manager.instance KS_ERR_BADPATH}}"
     */
    function getVar (
        path: string | string[],
        requestOutput: "OP_NAME" | "OP_TYPE" | "OP_VALUE" | "OP_TIMESTAMP" | "OP_STATE",
        cbfnc?: IKsCallback,
        responseFormat?: "text/tcl" | "text/ksx" | "text/plain"
    ): string;
    /**
     * Sets a KS Variable
     * @param path of the variable to set
     * @param {String} value to set (StringVec are Arrays)
     * @param {String} type variable type (for example "KS_VT_STRING") to set, null if no change
     * @param cbfnc callback function for a async request
     * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used). "text/tcl" if not supplied
     * @return true, "" or null
     */
    function setVar (
        path: string,
        value: string | string[],
        type: "KS_VT_BOOL" | "KS_VT_INT" | "KS_VT_UINT" | "KS_VT_SINGLE" | "KS_VT_DOUBLE" | "KS_VT_STRING" | "KS_VT_TIME" | "KS_VT_TIME_SPAN" | "KS_VT_STATE" | "KS_VT_STRUCT" | "KS_VT_BYTE_VEC" | "KS_VT_BOOL_VEC" | "KS_VT_INT_VEC" | "KS_VT_UINT_VEC" | "KS_VT_SINGLE_VEC" | "KS_VT_DOUBLE_VEC" | "KS_VT_STRING_VEC" | "KS_VT_TIME_VEC" | "KS_VT_TIME_SPAN_VEC" | "KS_VT_TIME_SERIES" | "KS_VT_STATE_VEC" | null,
        cbfnc?: IKsCallback,
        responseFormat?: "text/tcl" | "text/ksx" | "text/plain"
    ): "" | true | null
    /**
     * Rename a KS object
     * @param path of the object to rename
     * @param newname (optional with full path) of the object
     * @param cbfnc callback function for a async request
     * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used). "text/tcl" if not supplied
     * @return true, "" or null
     */
    function renameObjects (
        oldName: string,
        newName: string,
        cbfnc?: IKsCallback,
        responseFormat?: "text/tcl" | "text/ksx" | "text/plain"
    ): "" | true | null
    /**
     * Create a KS object
     * @param path of the object to create
     * @param classname full class name of the new object
     * @param cbfnc callback function for a async request
     * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used). "text/tcl" if not supplied
     * @return true, "" or null
     */
    function createObject (
        path: string,
        classname: string,
        cbfnc?: IKsCallback,
        responseFormat?: "text/tcl" | "text/ksx" | "text/plain"
    ): "" | true | null
    /**
     * Delete a KS object
     * @param path ob the object to delete
     * @param cbfnc callback function for a async request
     * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used). "text/tcl" if not supplied
     * @return true, "" or null
     */
    function deleteObject (
        path,
        cbfnc?: IKsCallback,
        responseFormat?: "text/tcl" | "text/ksx" | "text/plain"
    ): "" | true | null
    /**
     * Link two KS objects
     * @param pathA of the first object
     * @param pathB of the second object
     * @param portnameA  name of the port
     * @param cbfnc callback function for a async request
     * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used). "text/tcl" if not supplied
     * @return true, "" or null
     */
    function linkObjects (
        pathA: string,
        pathB: string,
        portnameA: string,
        cbfnc?: IKsCallback,
        responseFormat?: "text/tcl" | "text/ksx" | "text/plain"
    ): "" | true | null
    /**
     * Unlinks two KS objects
     * @param pathA of the first object
     * @param pathB of the second object
     * @param portnameA  name of the port
     * @param cbfnc callback function for a async request
     * @param responseFormat Mime-Type of requested response (probably "text/tcl", "text/ksx", "text/plain" used). "text/tcl" if not supplied
     * @return true, "" or null
     */
    function unlinkObjects (
        pathA: string,
        pathB: string,
        portnameA: string,
        cbfnc?: IKsCallback,
        responseFormat?: "text/tcl" | "text/ksx" | "text/plain"
    ): "" | true | null

    /**
     * Prints an info message on the website
     * @param text 
     */
    function log_info_onwebsite (
        text: string
    ): void;
    /**
     * Prints an error message on the website
     * @param text 
     */
    function log_error_onwebsite (
        text: string
    ): void;
    /**
     * returns the KS Response as an Array, or an empty Array
     * if the optional argument recursionDepth is > 0,  
     */
    function splitKsResponse (
        response: string,
        recursionDepth: number
    ): any[];
}
