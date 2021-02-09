# Quran-application

بِسْمِ ٱللّٰهِ الرَّحْمٰنِ الرَّحِيْمِ ,
السَّلاَمُ عَلَيْكُمْ وَرَحْمَةُ اللهِ وَبَرَكَاتُهُ

**Overview**

Quran app is an easy to use quran reader with support for translation in multiple languages and stream beautiful quran recitation . Made with Qt5 .

User can Access Prayer Times from Menu->Prayer Times.

Quran streaming only available in CLI mode now. Open terminal and type `qapp --h` to show help menu. 

This Project uses [nlohmann json](https://github.com/nlohmann/json) for json parsing .

![shot2](https://user-images.githubusercontent.com/77830098/105571013-b9375c00-5d72-11eb-9da7-67d1e7384796.png)
![shot3](https://user-images.githubusercontent.com/77830098/105571016-bd637980-5d72-11eb-86a2-bd70acb45954.png)


**Dependencies**

`libcurl4-openssl-dev , qt5-default, nlohmann json, libpmg123-dev, libao-dev`
* libcurl installation = `sudo apt install libcurl4-openssl-dev`
* Qt5 = `sudo apt install qt5-default`
* libmpg123 installation = `sudo apt install libmpg123-dev libmpg123-0`
* libao installation = `sudo apt install libao-dev libao4`

**Installation**

1. Install Dependencies
2. clone the repo and cd to repo folder
3. `mkdir build && cmake ..`
4. run `make` , it will compile the program
5. run `cd .. && sudo ./install.sh`
6. Open the app from application menu 

*Note: If you get GTK-Warning failed to load canberra-gtk-module , fix it by*
`sudo apt install libcanberra-gtk-module`
