cd ../

if exist "mapeditor.cpp" (move "mapeditor.cpp" "src/siai/editor/forms")

if exist "mapeditor.h" (move "mapeditor.h" "include/siai/editor/forms")

cd scripts

fix-mapeditor-include.py

echo "Done"

pause