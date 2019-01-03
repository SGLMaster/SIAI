import fileinput
import sys

for line in fileinput.input("../src/siai/editor/forms/mapeditor.cpp", inplace=1):
    if "mapeditor.h" in line:
        line = line.replace('"mapeditor.h"', '"editor/forms/mapeditor.h"')
    sys.stdout.write(line)
