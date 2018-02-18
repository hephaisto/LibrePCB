import sys

import librepcbpy as lp

try:
    symbol_uuid = sys.argv[1]
except IndexError:
    print("""USAGE: {} SYMBOL_UUID
    prints information about the symbol with the specified uuid
    currently only works for symbols in LibrePCB_Base
    """.format(sys.argv[0]))
    sys.exit(1)

## INIT
lp.init()

## find workspace
wp = lp.determineWorkspacePath()
print("using workspace path: '{}'".format(wp.toStr()))

## load symbol
s = lp.Symbol(wp.getPathTo("v0.1/libraries/local/LibrePCB_Base.lplib/sym/{}/".format(symbol_uuid)), False)
print("The symbol authored by '{}' has the following {} pins:".format(s.author, len(s.pins)))
for p in s.pins:
    print("* {}".format(p.name))

