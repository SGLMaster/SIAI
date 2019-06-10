import fileinput
import sys

for line in fileinput.input("../src/siai/ingress/ingress.cpp", inplace=1):
    if "ingress.h" in line:
        line = line.replace('"ingress.h"', '"ingress/ingress.h"')
    sys.stdout.write(line)
