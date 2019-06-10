cd ../

if exist "manager.cpp" (move "manager.cpp" "src/siai/manager/forms")

if exist "manager.h" (move "manager.h" "include/siai/manager/forms")

cd scripts

fix-manager-include.py

cd ../

if exist "ingress.cpp" (move "ingress.cpp" "src/siai/ingress")

if exist "ingress.h" (move "ingress.h" "include/siai/ingress")

cd scripts

fix-ingress-include.py

echo "Done"

pause