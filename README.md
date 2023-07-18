# QuranApp

بِسْمِ اللّٰهِ الرَّحْمٰنِ الرَّحِيْمِ ,
السَّلاَمُ عَلَيْكُمْ وَرَحْمَةُ اللهِ وَبَرَكَاتُهُ

**Overview**

Quran app is an easy to use quran reader with support for translation in multiple languages and stream beautiful quran recitation . Made with Qt5 .

User can Access Prayer Times from Menu->Prayer Times.

This Project uses [nlohmann json](https://github.com/nlohmann/json) for json parsing .

Windows Repo : https://github.com/Muslim-Programmers/QuranApp-Windows

**Screenshots**

![qapp1](https://user-images.githubusercontent.com/95064572/150300379-6c33af55-a0a3-4d37-a269-9ce5683cd888.png)
![qapp2](https://user-images.githubusercontent.com/95064572/150300402-2d2fd7aa-f465-48f9-86bb-a593ca1b4dd6.png)

**Dependencies**

**Debian** Derivatives :-

`libcurl4-openssl-dev , qt5 , nlohmann json, libsqlite3-dev`
* libcurl installation = `sudo apt install libcurl4-openssl-dev`
* Qt5 = `sudo apt install qtbase5-dev qtmultimedia5-dev libqt5multimedia5-plugins`
* libsqlite3 installation = `sudo apt install libsqlite3-dev`

**Installation**

1. Install Dependencies
2. clone the repo and cd to repo folder
3. `mkdir build && cd build && cmake ..`
4. run `make` , it will compile the program
5. run `cd .. && sudo ./install.sh`
6. Open the app from application menu 

*Note: If you get GTK-Warning failed to load canberra-gtk-module , fix it by*
`sudo apt install libcanberra-gtk-module`

Prebuilt binaries are available in releases page.
