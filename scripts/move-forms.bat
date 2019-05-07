cd ../

if exist "manager.cpp" (move "manager.cpp" "src/siai/manager/forms")

if exist "manager.h" (move "manager.h" "include/siai/manager/forms")

cd scripts

fix-manager-include.py

echo "Done"

pause