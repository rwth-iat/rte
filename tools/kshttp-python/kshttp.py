"""
This module provides some rudimentary classes and functions to interact with ACPLT/RTE over http.
This is not intended as a full fledged client but rather a tool for creating scripts for developing or
testing purposes.

Classes:
    Server: basic class that contains the address of a server and functions to access the base services
    several Enums (PMH, VT, OT, EPScope, OP, FMT): Lists possible argument values for some services

Functions:
    ks_get_server: get port of a server
    server_by_name: get a server object by name
    object_exists: check if an object is present
    list_ep: list contained elements of an object
    get_access: get access rights of an element
    get_instances: get instances of a class
    get_class: get class of an object
    get_base_class: get base class of a class
    is_subclass_instance: check if an instance is of a class or one of its subclasses
    get_all_subclasses: get all classes derived from a base class
    unlink_all: unlink all existing objects from an association
    print_instance_counts: list number of instances of a class or classes of a library
"""

from enum import Enum
from queue import SimpleQueue
from typing import Union, List, Any

import requests
from requests import RequestException


class PMH(Enum):
    """This class enumerates available placement hints for linking"""
    DEFAULT = 'DEFAULT'
    BEGIN = 'BEGIN'
    END = 'END'
    BEFORE = 'BEFORE'
    AFTER = 'AFTER'


class VT(Enum):
    """This class enumerates available variable types for setvar"""
    BOOL_VEC = 'KS_VT_BOOL_VEC'
    BOOL = 'KS_VT_BOOL'
    BYTE_VEC = 'KS_VT_BYTE_VEC'
    DOUBLE_VEC = 'KS_VT_DOUBLE_VEC'
    DOUBLE = 'KS_VT_DOUBLE'
    INT_VEC = 'KS_VT_INT_VEC'
    INT = 'KS_VT_INT'
    SINGLE_VEC = 'KS_VT_SINGLE_VEC'
    SINGLE = 'KS_VT_SINGLE'
    STRING_VEC = 'KS_VT_STRING_VEC'
    STRING = 'KS_VT_STRING'
    TIME_SPAN_VEC = 'KS_VT_TIME_SPAN_VEC'
    TIME_SPAN = 'KS_VT_TIME_SPAN'
    TIME_VEC = 'KS_VT_TIME_VEC'
    TIME = 'KS_VT_TIME'
    UINT_VEC = 'KS_VT_UINT_VEC'
    UINT = 'KS_VT_UINT'
    VOID = 'KS_VT_VOID'


class OT(Enum):
    """This class enumerates available response types for getep"""
    DOMAIN = 'OT_DOMAIN'
    VARIABLE = 'OT_VARIABLE'
    LINK = 'OT_LINK'
    ALL = 'OT_ANY'


class EPScope(Enum):
    """This class enumerates possible scopes for getep"""
    CHILD = 'children'
    PART = 'parts'


class OP(Enum):
    """This class enumerates available output fields for getep"""
    NAME = 'OP_NAME'
    TYPE = 'OP_TYPE'
    COMMENT = 'OP_COMMENT'
    ACCESS = 'OP_ACCESS'
    SEMANTIC = 'OP_SEMANTIC'
    CREATIONTIME = 'OP_CREATIONTIME'
    CLASS = 'OP_CLASS'
    TECHUNIT = 'OP_TECHUNIT'
    ASSOCIDENT = 'OP_ASSOCIDENT'
    ROLEIDENT = 'OP_ROLEIDENT'


class FMT(Enum):
    """This class enumerates available response formats"""
    PLAIN = 'plain'
    TCL = 'tcl'
    XML = 'ksx'
    JSON = 'json'


class Server:
    """
    Class to represent a server.

    :var str host: hostname of serber
    :var int port: port of server

    Functions:
        get_var: get variable from server
        set_var: set variable on server
        create: create an object
        delete: delete an object
        get_ep: get engineering properties
        link: link an association
        unlink: unlink an association
    """
    def __init__(self, host: str, port: int):
        self.host = host
        self.port = port

    def get_var(self, path: str, fmt: FMT = FMT.PLAIN) -> Union[str, List[str]]:
        """
        Get a variable from the server.

        :param str path: path of variable to get
        :param FMT fmt: format of response (default plain)
        :return: list of values for plain format or string with response for other formats
        :raises RequestException: if request did not return status code 200
        """
        args = list()
        args.append(('format', fmt.value))
        args.append(('path', path))

        url = 'http://{host}:{port}/getVar'.format(host=self.host, port=self.port)

        r = requests.get(url, params=args)

        if r.status_code != 200:
            print(r.text)
            raise RequestException('bad return code')

        if fmt == FMT.PLAIN:
            return r.text.rstrip(';').split('; ')
        else:
            return r.text

    def set_var(self, path: str, value: Union[List[Any], Any], vartype: VT = None) -> bool:
        """
        Set a variable on the server.

        :param str path: path of variable to set
        :param Any value: new value for the variable
        :param VT vartype: optional variable type for any variables
        :return:
        """
        args = list()
        args.append(('format', FMT.PLAIN.value))
        args.append(('path', path))
        if not isinstance(value, list):
            args.append(('newvalue', str(value)))
        else:
            args.append(('newvalue', list_python_to_tcl(value)))

        if vartype is not None:
            args.append(('vartype', vartype.value))

        url = 'http://{host}:{port}/setVar'.format(host=self.host, port=self.port)

        r = requests.get(url, params=args)

        if r.status_code != 200:
            print(r.text)
            return False

        return True

    def create(self, factory: str, path: str) -> bool:
        """
        Create object on the server.

        :param str factory: path to class object to use as factory
        :param str path: path for new object
        :return: returns if the create call was successful
        """
        args = list()
        args.append(('format', FMT.PLAIN.value))
        args.append(('factory', factory))
        args.append(('path', path))

        url = 'http://{host}:{port}/createObject'.format(host=self.host, port=self.port)

        r = requests.get(url, params=args)

        if r.status_code != 200:
            print(r.text)
            return False

        return True

    def delete(self, path: str) -> bool:
        """
        Delete object on the server.

        :param str path: path of object to delete
        :return: returns if the delete call was successful
        """
        args = list()
        args.append(('format', FMT.PLAIN.value))
        args.append(('path', path))

        url = 'http://{host}:{port}/deleteObject'.format(host=self.host, port=self.port)

        r = requests.get(url, params=args)

        if r.status_code != 200:
            print(r.text)
            return False

        return True

    def get_ep(self, path: str, r_type: OT = OT.DOMAIN, output: List[OP] = None, scope: EPScope = None,
                  mask: str = None, fmt: FMT = FMT.TCL) -> str:
        """
        Get engineering properties of object.

        :param str path: path of object to get ep from
        :param OT r_type: type of elements to return ep for (default DOMAIN)
        :param List[OP] output: list of properties to return (None returns server default)
        :param EPScope scope: restrict scope to children or parts (optional)
        :param str mask: return only matching elements (supports * and ?; default no mask)
        :param FMT fmt: format of response (default is tcl)
        :return: response text from the call. no parsing is done
        :raises RequestException: if request did not return status code 200
        """
        args = list()
        args.append(('format', fmt.value))
        args.append(('path', path))
        args.append(('requestType', r_type.value))

        if output is not None:
            args.extend([('requestOutput', o.value) for o in output])
        if scope is not None:
            args.append(('scopeFlags', scope.value))
        if mask is not None:
            args.append(('nameMask', mask))

        url = 'http://{host}:{port}/getEP'.format(host=self.host, port=self.port)

        r = requests.get(url, params=args)

        if r.status_code != 200:
            raise RequestException(r.text)

        return r.text

    def link(self, path: str, other: str, hint: PMH = PMH.DEFAULT, hint_rel: str = '',
                o_hint: PMH = PMH.DEFAULT, o_hint_rel: str = '') -> bool:
        """
        Link an association with an object.

        :param str path: path to association specifier
        :param str other: path to object to link with
        :param PMH hint: placement hint for object in association (default DEFAULT)
        :param str hint_rel: path to relative positioning element for hint
        :param PMH o_hint: placement hint for object in path in association of other (default DEFAULT)
        :param str o_hint_rel: path to relative positioning element for o_hint
        :return: returns if the link call wass successful
        """
        args = list()
        args.append(('format', FMT.PLAIN.value))
        args.append(('path', path))
        args.append(('element', other))

        if hint != PMH.DEFAULT or o_hint != PMH.DEFAULT:
            args.append(('placementHint', hint.value))
        if o_hint != PMH.DEFAULT:
            args.append(('oppositePlacementHint', o_hint.value))

        if hint in (PMH.BEFORE, PMH.AFTER):
            args.append(('placePath', hint_rel))
        if o_hint in (PMH.BEFORE, PMH.AFTER):
            args.append(('oppositePlacePath', o_hint_rel))

        url = 'http://{host}:{port}/link'.format(host=self.host, port=self.port)

        r = requests.get(url, params=args)
        print(r.url)

        if r.status_code != 200:
            print(r.text)
            return False

        return True

    def unlink(self, path: str, other: str) -> bool:
        """
        Unlink an object from an association.

        :param str path: path to association specifier
        :param str other: path ot object to unlink form association
        :return: returns if call to unlink was successful
        """
        args = list()
        args.append(('format', FMT.PLAIN.value))
        args.append(('path', path))
        args.append(('element', other))

        url = 'http://{host}:{port}/unlink'.format(host=self.host, port=self.port)

        r = requests.get(url, params=args)

        if r.status_code != 200:
            print(r.text)
            return False

        return True


def list_tcl_to_python(tcl_list: str) -> List:
    """FIXME to be implemented"""
    pass


def list_python_to_tcl(python_list: List) -> str:
    """converts a python list into a tcl list string"""
    return ' '.join(['{' + str(i) + '}' for i in python_list])


def ks_get_server(host: str, server: str) -> int:
    """
    Get port of a server form MANAGER.

    :param str host: hostname of the MANAGER
    :param str server: server name to get port for
    :return: port of requested server
    """
    args = list()
    args.append(('format', FMT.PLAIN.value))
    args.append(('servername', server))

    url = 'http://{host}:{port}/getServer'.format(host=host, port=7509)

    r = requests.get(url, params=args)

    if r.status_code != 200:
        print(r.text)
        raise RequestException('bad return code')

    return int(r.text.split(';', maxsplit=1)[0])


def server_by_name(server: str, host: str = 'localhost') -> Server:
    """
    Get server object by name form MANAGER

    :param str server: name of server to get
    :param str host: hostname of the MANAGER (default: localhost)
    :return: server object for requested server
    """
    return Server(host, ks_get_server(host, server))


def object_exists(server: Server, inst: str) -> bool:
    """
    Test if an object exists.

    :param Server server: server object
    :param str inst: path to object
    :return: returns if object exists
    """
    try:
        server.get_ep(inst, output=[OP.NAME], fmt=FMT.PLAIN)
    except RequestException:
        return False

    return True


def list_ep(server: Server, path: str, r_type: OT = OT.DOMAIN, scope: EPScope = None) -> List[str]:
    """
    List elements under an object fitting a type.

    :param Server server: server object
    :param str path: path to object
    :param OT r_type: type of ep to return (default is domain)
    :param EPScope scope: restrict scope to children or parts
    :return: list of sub elements
    """
    ls = server.get_ep(path, r_type=r_type, output=[OP.NAME], scope=scope, fmt=FMT.PLAIN).rstrip(';').split('; ')
    if len(ls[0]) == 0:
        return list()

    return ls


def get_access(server: Server, path: str, element: str) -> List[str]:
    """
    Returns access for an element of an object.

    :param Server server: server object
    :param str path: path to object
    :param str element: name of contained element
    :return: list of access rights
    """
    access = server.get_ep(path, r_type=OT.ALL, output=[OP.ACCESS], mask=element, fmt=FMT.TCL).rstrip(
        '}').lstrip('{').split(' ')
    if len(access[0]) == 0:
        return list()
    return access


def get_instances(server: Server, cls: str) -> List[str]:
    """
    Get list of instances of a class.

    :param Server server: server object
    :param str cls: path to class
    :return: list of paths of instances
    """
    try:
        inst = server.get_var(cls + '.instance')
        if len(inst) == 1 and len(inst[0]) == 0:
            return list()
    except RequestException:
        return list()

    return inst


def get_class(server: Server, path: str) -> str:
    """
    Get class of an object.

    :param Server server: server object
    :param str path: path to object
    :return: path to class of object
    """
    return server.get_var(path + '.objectclass')[0]


def get_base_class(server: Server, cls: str) -> str:
    """
    Get base class of another class

    :param Server server: server object
    :param str cls: path to class
    :return: path to base class
    """
    return server.get_var(cls + '.baseclass')[0]


def is_subclass_instance(server: Server, path: str, base_class: str) -> bool:
    """
    Check if instance is of a base class.

    :param Server server: server object
    :param str path: path to instance
    :param str base_class: path to base class
    :return: returns if path is an instance of base_class or one of its sub classes
    """
    cls = get_class(server, path)
    if cls is base_class:
        return True

    while '/ov/object' not in cls:
        cls = get_base_class(server, cls)
        if cls == base_class:
            return True

    return False


def get_all_subclasses(server: Server, cls: str) -> List['str']:
    """
    Get all subclasses of a class

    :param Server server: server object
    :param str cls: path to class
    :return: list of all sub classes of cls
    """
    q = SimpleQueue()
    q.put(cls)

    sub_cls: List[str] = list()

    while not q.empty():
        try:
            for sub in server.get_var(cls + '.derivedclass'):
                if len(sub) > 0:
                    sub_cls.append(sub)
                    q.put(sub)
        except RequestException:
            return list()

    return sub_cls


def unlink_all(server: Server, path: str) -> bool:
    """
    Unlink all associations that are unlinkable

    :param Server server: server object
    :param str path: path to obect
    :return: returns success of unlinking
    """
    for other in server.get_var(path):
        if len(other) > 0:
            if not server.unlink(path, other):
                return False

    return True


def delete_object(server, path: str, is_part: bool = False) -> bool:
    """
    Delete an object and all of its children while also preparing objects to become deletable.

    :param Server server: server object
    :param str path: path to object to delete
    :param bool is_part: internal flag when processing parts
    :return: success of operation
    """
    # FIXME still fails to get access for deleting
    # delete child objects
    for obj in list_ep(server, path, r_type=OT.DOMAIN, scope=EPScope.CHILD):
        if not delete_object(server, path + '/' + obj):
            return False

    # delete parts
    for part in list_ep(server, path, r_type=OT.DOMAIN, scope=EPScope.PART):
        if not delete_object(server, path + '/' + part, True):
            return False

    # unlink all associations
    for assoc in list_ep(server, path, r_type=OT.LINK):
        if 'KS_AC_UNLINKABLE' not in get_access(server, path, assoc):
            continue
        if not unlink_all(server, path + '.' + assoc):
            return False

    if is_part:
        return True

    return server.delete(path)


def print_instance_counts(server: Server, lib: str = None, cls: str = None):
    """
    Print instance count for classes of a library or a single class.

    :param Server server: server object
    :param str lib: name of library
    :param str cls: path to a single class
    :return: None
    """
    if lib is None and cls is None:
        return

    if cls is not None:
        print(cls + ': ', len(get_instances(server, cls)))

    if lib is not None:
        base = None
        for l in server.get_var('/acplt/ov/library.instance'):
            if l.endswith('/' + lib):
                base = l

        if base is None:
            return

        ep = server.get_ep(base, output=[OP.NAME, OP.CLASS], fmt=FMT.TCL).lstrip('{').rstrip('}').split('} {')
        cls = list()
        for element in ep:
            name, c = element.split(' ')
            if c == '/acplt/ov/class':
                cls.append(name)

        for c in cls:
            print(c + ': ', len(get_instances(server, base + '/' + c)))
