/***
   * Project Name : Quran app
   * Description : an easy to use Quran Reading program in Qt5, supports translations in indian languages as well as quran audio streaming
   *               Specs :-  Multi-language , stream quran
   * Build-Dependencies : libcurl , qt5 libs, nlohmann json : https://github.com/nlohmann/json/ , libao-dev , libmpg123-dev
   * Developer : Nashid P , Member of Muslim Programmers Community
   *
***/

#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <cstdio>
#include "audio_stream.hpp"
#include "window.hpp"

void help_menu();
int check_option(int argc, char *argv[], int x);
bool isSurah(char *arg);
bool isInt(char *arg);
void process_option(int argc, char *argv[], int x);

int main(int argc, char *argv[])
{
    // Initiate Qt Application
    QApplication qapp(argc, argv);
    Window *window;                                         // Main Window
    QPixmap pixmap("/opt/qapp/resources/qapp-splash.jpg");            // Image to show in Splash Screen
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint); // Splash Screen object
    
    // Check if there is command-line arguements provided

    if(argc == 1)
    {
        // If no command-line arguements provided , this block is executed

        window = new Window;
        splash.show(); // Show Splash Screen
        QTimer::singleShot(3000, &splash, &QWidget::close); // Keep Splash Screen for 3 seconds
        window->show(); // Show Main Window
    }
    else
    {
        // when command-line arguements are provide , this block is executed

        for(int x=1;x<argc;x++)
        {
            // check for "--" in arguement

            if(argv[x][0] == '-' && argv[x][1] == '-' && argc >= 2)
            {
                x = check_option(argc, argv, x);
                break;
            }
            else
            {
                help_menu();
                exit(0);
            }
        }
    }
    return qapp.exec();
}

void help_menu()
{
    // command-line help menu , accessed using '--h' arguement

    std::cout << " Usage: qapp [OPT] [arg] " << std::endl;
    std::cout << " OPTions:- " << std::endl;
    std::cout << "\t --s (Stream): " << std::endl;
    std::cout << "\t\t[arg]:- " << std::endl;
    std::cout << "\t\t\t[surah] stream complete surah " << std::endl;
    std::cout << "\t\t\tEg:- " << std::endl;
    std::cout << "\t\t\t   qapp --s 1 , stream surah Al-Fatiha , recitation of Mishary Alafasy \n" << std::endl;
    std::cout << "\t --h (Help): " << std::endl;
    std::cout << "\t\t show this help menu and exit \n" << std::endl;
}

int check_option(int argc, char *argv[], int x)
{
    if(argv[x][2] == 'h') // --h
    {
        help_menu(); // call help_menu()
        exit(0);     // jump to return 0
    }
    else if(argv[x][2] == 's') // --s
    {
        // Quran API URL
        std::string url_surah = "https://api.alquran.cloud/v1/surah/";
        
        if(!isInt(argv[x+1]))
        {
            // if arguement is not number show error message
                
            std::cout << "\n\033[1;31m [surah] must be an integer , provided value = " << argv[x+1] << "\033[0m" << std::endl;
            exit(0);   
        }
        else
        {
            std::cout << "\033[1;32m Streaming .... \033[0m";
            url_surah.append(argv[x+1]);                        // append arguement to API URL
            url_surah.append("/ar.alafasy");
            audio_stream newstream(url_surah);                  // Initiate Audio Stream
            newstream.process_stream();                         // Play Stream
            std::cout << "\033[1;32m Stream ended \033[0m " << std::endl;
            exit(0);
        }

    }
    else 
    {
        std::cout << "\033[1;31m Invalid option provided or insufficient arguements , use --h to show help menu \033[0m" << std::endl; 
        exit(0);
    }
    return x;
}

bool isSurah(char *arg)
{
    for(int i=0;arg[i]!='\0';i++)
    {
        // loop till the end of the string

        if(arg[i] == ':')
            return false;
    }
    return true;
}

bool isInt(char *arg)
{
    int arglen = strlen(arg); // get arguement string length
    int intnum = 0;           // Number of Integers   
    for(int i=0;arg[i]!= '\0';i++)
    {
        
        if(arg[i] >= '0' && arg[i] <= '9')
        {
            // If each character is between 0 and 9 Increment intnum

            intnum++;
        }

    }
    if(intnum == arglen)
    {
        return true;
    }
    return false;
}
