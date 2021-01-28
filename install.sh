FILE=./build/qapp
if [ -f "$FILE" ]; then
	mkdir -p $HOME/.local/share/qapp/ 
	cp ./build/qapp $HOME/.local/bin/
	cp ./resources/qapp-splash.jpg $HOME/.local/share/qapp/
	cp ./resources/qapp-198x198.png $HOME/.local/share/qapp/
	cp ./resources/icon.png $HOME/.local/share/qapp/
	cp qapp.desktop $HOME/.local/share/applications/ 
	echo "Successfully Installed"
	echo "Open Quran app from Application Menu or type qapp --h "
else
	echo "$FILE does not exist , please compile the program and run this script again"
fi


