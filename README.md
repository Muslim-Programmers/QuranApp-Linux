# Quran-application

بِسْمِ ٱللّٰهِ الرَّحْمٰنِ الرَّحِيْمِ ,
السَّلاَمُ عَلَيْكُمْ وَرَحْمَةُ اللهِ وَبَرَكَاتُهُ

Quran app is an easy to use quran reader with support for translation in multiple languages and stream beautiful quran recitation . Made with Qt5 .

**Dependencies**

`libcurl4-openssl-dev , qt5-default, nlohmann json, libpmg123-dev, libao-dev`
* libcurl installation = `sudo apt install libcurl4-openssl-dev`
* nlohmann json = https://github.com/nlohmann/json/releases , Download the latest release and put it in the source folder . Make sure header name is json.hpp
* Qt5 = `sudo apt install qt5-default`
* libmpg123 installation = `sudo apt install libmpg123-dev libmpg123-0`
* libao installation = `sudo apt install libao-dev libao4`

**Installation**

1. Install Dependencies
2. clone the repo and cd to repo folder
3. `mkdir build && cmake ..`
4. run `make` , it will compile the program
5. run the program `./qapp --h`

*Note: If you get GTK-Warning failed to load canberra-gtk-module , fix it by*
`sudo apt install libcanberra-gtk-module`
