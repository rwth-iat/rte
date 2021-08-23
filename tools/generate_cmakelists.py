
import argparse
import os
from plot_library_dependencies import find_libs 

parser = argparse.ArgumentParser("Find OV libraries and generate CMakeLists files for them")
parser.add_argument('directories', metavar="DIRECTORY", type=str, nargs='+')
args = parser.parse_args()

libs = find_libs(args.directories)

for library in libs.values():
    cmake_path = os.path.join(library.location, "CMakeLists.txt")
    if os.path.isfile(cmake_path):
        continue

    with open(cmake_path, 'w') as f:
        f.write("""
add_ov_library({libname})
ov_library_includes({libname} {includes})

"""
            .format(libname=library.name, includes=" ".join(library.includes)))
