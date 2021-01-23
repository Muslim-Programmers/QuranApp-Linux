# Quran-application

بِسْمِ ٱللّٰهِ الرَّحْمٰنِ الرَّحِيْمِ ,
السَّلاَمُ عَلَيْكُمْ وَرَحْمَةُ اللهِ وَبَرَكَاتُهُ

Quran app is an easy to use quran reader with support for translation in multiple languages and stream beautiful quran recitation . Made with Qt5 .

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

quran streaming only available in CLI mode now.

*Note: If you get GTK-Warning failed to load canberra-gtk-module , fix it by*
`sudo apt install libcanberra-gtk-module`


![](https://cdn.discordapp.com/attachments/802126045804232704/802132543612780594/unknown.png)
