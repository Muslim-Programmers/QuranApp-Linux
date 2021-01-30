FILE=./build/qapp
if [ -f "$FILE" ]; then
	mkdir -p /usr/share/qapp/ 
	sudo cp ./build/qapp /usr/local/bin/
	sudo cp ./resources/qapp-splash.jpg /usr/share/qapp/
	sudo cp ./resources/qapp-198x198.png /usr/share/qapp/
	sudo cp ./resources/icon.png /usr/share/qapp/
	sudo cp qapp.desktop /usr/share/applications/ 
	sudo echo "Successfully Installed"
	sudo echo "Open Quran app from Application Menu or type qapp --h "
else
	echo "$FILE does not exist , please compile the program and run this script again"
fi


