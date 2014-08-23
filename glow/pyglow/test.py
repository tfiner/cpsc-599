#!/usr/bin/env python

cmdLine = "glow --input scenes/mud.json --output=mud-py.png --verbose"

if __name__ == "__main__":
    import sys
    sys.path.append('debug')

    import glow
    glow.run(cmdLine.split())
