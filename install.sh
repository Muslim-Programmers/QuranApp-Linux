FILE=./build/qapp
if [ -f "$FILE" ]; then
	sudo mkdir -p /opt/Qapp/
	sudo cp ./build/qapp /usr/bin/
	sudo cp ./resources/qapp-splash.jpg /opt/Qapp/
	sudo cp ./resources/qapp-198x198.png /opt/Qapp/
	sudo cp ./resources/icon.png /opt/Qapp/
	sudo cp qapp.desktop /usr/share/applications/
	echo "Successfully Installed"
	echo "Open Quran app from Application Menu or type qapp --h "
else
	echo "$FILE does not exist , please compile the program and run this script again"
fi


