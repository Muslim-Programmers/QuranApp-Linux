FILE=./build/qapp
if [ -f "$FILE" ]; then
	mkdir -p /opt/qapp/ 
	sudo cp ./build/qapp /usr/bin/
	sudo cp -r resources/ /opt/qapp/
	sudo cp -r qdarkstyle/ /opt/qapp/resources/
	sudo cp qapp.desktop /usr/share/applications/ 
	sudo echo "Successfully Installed"
	sudo echo "Open Quran app from Application Menu or type qapp --h "
else
	echo "$FILE does not exist , please compile the program and run this script again"
fi


