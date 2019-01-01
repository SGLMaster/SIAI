cd ../

echo -n "Select between DEBUG, RELEASE or CLEAN with D, R or C: "
old_stty_cfg=$(stty -g)
answer=$(head -c 1);

if echo "$answer" | grep -iq "R" ;then
	mingw32-make -j 2 DEBUG=0;
elif echo "$answer" | grep -iq "C" ;then
	mingw32-make DEBUG=0 clean;
	mingw32-make DEBUG=1 clean;
else
	mingw32-make -j 2 DEBUG=1;
fi

echo "Finish building!"