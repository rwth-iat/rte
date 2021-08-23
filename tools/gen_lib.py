"""
This is a script to generate a new ov library.

usage: gen_lib.py [-I|--include <lib>] [-s|--super <super class>] [-f|--force] -m|--main <main class> <lib name>

positional arguments:
  <lib name>                 name of new library

options:
  -p  --path <path>          base path where library is created
  -I  --include <lib>        include library <lib> in new library
  -s  --super <super class>  make <main class> a subclass of <super class>
  -f  --force                overwrite existing files
  -m  --main <main class>    create main class with name <main class>
"""
import argparse
import os
import re
import sys
from typing import List


class NewLibrary:
    def __init__(self, base_path: str, lib_name: str, main_class: str, super_class: str = 'ov/object',
                 includes: List[str] = None):

        self._base_path: str = base_path
        self._lib_path: str = f'{base_path}/{lib_name}'
        self._lib_name: str = lib_name
        self._main_class: str = main_class
        self._super_class: str = super_class
        self._force = False
        if includes is None:
            self._includes = list()
        else:
            self._includes: List[str] = includes

    def generate(self, force: bool = False):
        """generate structure and model files"""
        if not self.validate():
            print('[ERROR] validation of parameters failed', file=sys.stderr)
            sys.exit(1)
        self._force = force

        self.create_structure()
        self.write_cmake()
        self.write_ovm()
        self.write_ovf()
        self.write_ovt()

    def validate(self) -> bool:
        """check arguments for validity"""
        valid = True

        if not re.fullmatch('[A-Z][A-Z0-9]*', self._lib_name, flags=re.IGNORECASE):
            print(f"[ERROR] invalid library name: {self._lib_name}", file=sys.stderr)
            valid = False

        if not re.fullmatch('[A-Z][A-Z0-9]*/[A-Z][A-Z0-9]*', self._super_class, flags=re.IGNORECASE):
            print(f"[ERROR] invalid super class: {self._super_class}", file=sys.stderr)
            valid = False

        if not re.fullmatch('[A-Z][A-Z0-9]*', self._main_class, flags=re.IGNORECASE):
            print(f"[ERROR] invalid main class name: {self._main_class}", file=sys.stderr)
            valid = False

        super_lib = self._super_class.split('/')[0] if '/' in self._super_class else None
        for lib in self._includes:
            if not re.fullmatch('[A-Z][A-Z0-9]*', lib, flags=re.IGNORECASE):
                print(f"[ERROR] invalid include: {lib}", file=sys.stderr)
                valid = False

        if super_lib is not None and super_lib not in self._includes:
            self._includes.append(super_lib)
            print(f"[WARNING] super class library added to includes: {super_lib}", file=sys.stderr)

        return valid

    def write_ovm(self):
        """generate ovm file"""
        ovm_path = f'{self._lib_path}/model/{self._lib_name}.ovm'
        if os.path.isfile(ovm_path) and not self._force:
            print(f'[WARNING] ovm file already exists: {ovm_path}; use force to overwrite', file=sys.stderr)
            return

        if self._super_class == 'fb/functionblock':
            example_operation = 'typemethod  : C_FUNCTION <FB_FNC_TYPEMETHOD>;'
        else:
            example_operation = 'constructor : C_FUNCTION <OV_FNC_CONSTRUCTOR>;'

        includes = '\n'.join([f'#include "{lib}.ovm"' for lib in ['ov', *self._includes]])

        with open(ovm_path, 'w') as file:
            file.write('{includes}\n'
                       '\n'
                       'LIBRARY {libname}\n'
                       '	VERSION   = "V0.1 (DD-Mon-YYYY)";\n'
                       '	AUTHOR    = "";\n'
                       '	COPYRIGHT = "";\n'
                       '	COMMENT   = "";\n'
                       '\n'
                       '	/**\n'
                       '		Documentation of {main_class}.\n'
                       '		@note Javadoc syntax is allowed here.\n'
                       '		@attention Changes in this file will only generate new code in ./source/sourcetemplates/. Code in ./source/ is not overwritten. \n'
                       '	*/\n'
                       '	CLASS {main_class} : CLASS {base_class}\n'
                       '		IS_INSTANTIABLE;\n'
                       '		VARIABLES\n'
                       '			// Add variables here. The syntax is:\n'
                       '			// <Variable name> : <Datatype> [<Accessors>] [<FLAGS>] [<Comment>] [<INITIALVALUE>]\n'
                       '			// Example:\n'
                       '			// varName : STRING HAS_GET_ACCESSOR COMMENT="Description of varName.";\n'
                       '			// Example for a fb/functionblock based class\n'
                       '			// inputNAme : UINT HAS_SET_ACCESSOR FLAGS = "i" COMMENT = "Input of type UINT" INITIALVALUE = 5;\n'
                       '		END_VARIABLES;\n'
                       '		OPERATIONS\n'
                       '			// Declare functions here. You may use your own function types, e.g. MY_FUNC,\n'
                       '			// if MY_FUNC is declared in {libname}.ovf. Implement the functions in base.c.\n'
                       '			// fb/functionblock based classes have to implement typemethod\n'
                       '			{example_operation}\n'
                       '		END_OPERATIONS;\n'
                       '	END_CLASS;\n'
                       '\n'
                       '	/**\n'
                       '		Add associations and their documentation here.\n'
                       '		Association syntax is:\n'
                       '	*/\n'
                       '	//ASSOCIATION assocName : ONE_TO_MANY\n'
                       '	//	PARENT parentVarName : CLASS ov/object;\n'
                       '	//	CHILD  childVarName : CLASS ov/object;\n'
                       '	//END_ASSOCIATION;\n'
                       '\n'
                       'END_LIBRARY;\n'.format(libname=self._lib_name, main_class=self._main_class,
                                               base_class=self._super_class,
                                               includes=includes, example_operation=example_operation))

    def write_ovf(self):
        """generate ovf file"""
        ovf_path = f'{self._lib_path}/model/{self._lib_name}.ovf'
        if os.path.isfile(ovf_path) and not self._force:
            print(f'[WARNING] ovt file already exists: {ovf_path}; use force to overwrite', file=sys.stderr)
            return

        with open(ovf_path, 'w') as file:
            file.write('#ifndef {libname}_OVF_INCLUDED\n'
                       '#define {libname}_OVF_INCLUDED\n'
                       '\n'
                       '/* \n'
                       '	Declare your C functions here. \n'
                       '	Example:\n'
                       '	typedef OV_DLLFNCEXPORT void MY_FUNC(\n'
                       '		OV_STRING varName\n'
                       '	);\n'
                       '*/\n'
                       '\n'
                       '#endif\n'.format(libname=self._lib_name))

    def write_ovt(self):
        """generate ovt file"""
        ovt_path = f'{self._lib_path}/model/{self._lib_name}.ovt'
        if os.path.isfile(ovt_path) and not self._force:
            print(f'[WARNING] ovf file already exists: {ovt_path}; use force to overwrite', file=sys.stderr)
            return

        with open(ovt_path, 'w') as file:
            file.write('#ifndef {libname}_OVT_INCLUDED\n'
                       '#define {libname}_OVT_INCLUDED\n'
                       '\n'
                       '/* \n'
                       '	Define your data types here. \n'
                       '*/\n'
                       '\n'
                       '#endif\n'.format(libname=self._lib_name))

    def write_cmake(self):
        """generate CMakeList.txt"""
        cmake_path = f'{self._lib_path}/CMakeList.txt'
        if os.path.isfile(cmake_path) and not self._force:
            print(f'[WARNING] CMakeList.txt already exists: {cmake_path}; use force to overwrite', file=sys.stderr)
            return

        if len(self._includes) == 0:
            includes = 'ov'
        else:
            includes = ' '.join(self._includes)
        with open(cmake_path, 'w') as file:
            file.write('\n'
                       'add_ov_library({lib_name})\n'
                       'ov_library_includes({includes})\n'.format(lib_name=self._lib_name, includes=includes))

    def create_structure(self):
        """generate library structure"""
        os.makedirs(self._lib_path, exist_ok=True)

        os.makedirs(f'{self._lib_path}/model', exist_ok=True)
        os.makedirs(f'{self._lib_path}/include', exist_ok=True)
        os.makedirs(f'{self._lib_path}/source', exist_ok=True)


def main():
    # define script arguments
    parser = argparse.ArgumentParser(description='generate template for new ov library', prog='gen_lib.py')
    parser.add_argument('-p', '--path', metavar='path', default=None,
                        help='root path for new libraries; default: cwd()')
    parser.add_argument('-f', '--force', action='store_true', help='overwrite existing files')
    parser.add_argument('-I', '--include', action='append', dest='include', metavar='lib', help='include ov libraries')
    parser.add_argument('-s', '--super', metavar='super', default='ov/object',
                        help='super class of main class; default: "ov/object"')
    parser.add_argument('-m', '--main', metavar='main', required=True, help='main class of new library')
    parser.add_argument('name', metavar='lib name', help='name of new library')

    # parse arguments
    args = parser.parse_args()

    if args.path is None:
        args.path = os.getcwd()
    else:
        args.path = os.path.abspath(args.path)

    # generate template
    lib = NewLibrary(args.path, args.name, args.main, super_class=args.super, includes=args.include)
    lib.generate(args.force)


if __name__ == '__main__':
    main()
