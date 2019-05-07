import fileinput
import sys

for line in fileinput.input("../src/siai/manager/forms/manager.cpp", inplace=1):
    if "manager.h" in line:
        line = line.replace('"manager.h"', '"manager/forms/manager.h"')
    sys.stdout.write(line)
