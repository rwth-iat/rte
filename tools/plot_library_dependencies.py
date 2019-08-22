#!/usr/bin/env python3
"""
A simple python script to find OV libraries in multiple directories and plot a dependency graph of the libraries.

It uses Python's ow.walk() to recursively find `.ovm` in all provided directories. Afterwards it searches those files
for `#include "*.ovm"` lines and plots a dependency graph using graphviz and the pygraphviz module. The libraries are
colored differently according to the root directory, in which they have been found.

Libraries are only parsed once: If the same library is found multiple times, only the first definition is considered for
plotting (color and dependencies).

Simple Usage:
```bash
python3 plot_library_dependencies.py DIRECTORY1 DIRECTORY2 ...
```

If you want to specify the output filename and format, use the additional `--output` option. It defaults to
"libraries.svg".
```bash
python3 plot_library_dependencies.py --output dependencies.pdf DIRECTORY1 DIRECTORY2 ...
```
"""

from typing import Dict, List
import re
import os
import os.path
import argparse

import pygraphviz as pgv

BLACKLIST = ["ov", "fb"]
COLORS = ["#000000", "#0000bb", "#bb0000", "#00bb00", "#008888", "#880088", "#888800"]

def main():
    """
    Main function of this script.

    It parses command line arguments, uses `find_libs()` to search for OV libraries and plots the dependency graph using
    `output_graphviz()` and pygraphviz' `.draw()` method.
    """
    parser = argparse.ArgumentParser("Find OV libraries and plot a depedency graph")
    parser.add_argument('directories', metavar="DIRECTORY", type=str, nargs='+')
    parser.add_argument('--output', '-o', default="libraries.svg")
    args = parser.parse_args()

    libs = find_libs(args.directories)
    graph = output_graphviz(libs)
    graph.graph_attr['overlap']="false"
    graph.graph_attr['rankdir']="TB"
    graph.graph_attr['remincross']="true"
    graph.graph_attr['ranksep']="1.2"
    graph.draw(args.output, prog="dot")


class Library:
    """
    Represents an OV library with its name (as found in the ovm file's file name), its dependencies (inlcuded ovm
    files), its storage location and a color to be used for plotting it.
    """
    def __init__(self, name: str, includes: List[str] = [], location = "", color = "#000000"):
        self.name = name
        self.includes = includes
        self.location = location
        self.color = color


def find_libs(root_directories: List[str]) -> Dict[str, Library]:
    """
    Find libraries within a list of directories.

    Each directory is traversed recursively to search for files matching "/model/*.ovm". The ovm file's names is
    considered to be the library name.
    """
    libs = {}
    for root, color in zip(root_directories, COLORS):
        for (dirpath, _, filenames) in os.walk(root):
            models = [f for f in filenames if f.endswith('.ovm')]
            if models and os.path.basename(dirpath) == "model":
                ovm_filename = models[0]
                lib_name = ovm_filename[:-4]
                if lib_name.lower() not in libs:
                    libs[lib_name.lower()] = Library(
                        lib_name,
                        parse_includes(os.path.join(dirpath, ovm_filename)),
                        os.path.dirname(dirpath),
                        color)
    return libs


INCLUDE_REGEX = re.compile(r"^\s*#include\s+\"(\w+)\.ovm\"\s*$")

def parse_includes(ovm_file: str) -> List[str]:
    """
    Get a list of included OV libraries' names by parsing the ovm file at the given path.
    """
    includes = []
    with open(ovm_file, errors='replace') as f:
        for line in f.readlines():
            match = INCLUDE_REGEX.match(line)
            if match:
                includes.append(match.group(1))
    return includes


def output_graphviz(libs: Dict[str, Library]) -> pgv.AGraph:
    """
    Generate a pygraphviz Graph from the list of Library objects
    """
    G = pgv.AGraph(directed=True)
    for lib in libs.values():
        if lib.name not in BLACKLIST:
            G.add_node(lib.name.lower(), label=lib.name, tooltip=lib.location, shape="box", color=lib.color, style="filled", fontcolor="white")

    # Add missing dependencies
    for lib in libs.values():
        for inc in lib.includes:
            if inc.lower() not in libs and inc.lower() not in BLACKLIST:
                G.add_node(inc.lower(), label=inc, shape="box", color="#888888", style="dashed")

    for lib in libs.values():
        for inc in lib.includes:
            if lib.name.lower() not in BLACKLIST and inc.lower() not in BLACKLIST:
                G.add_edge(lib.name.lower(), inc.lower())
    return G


if __name__ == "__main__":
    main()
