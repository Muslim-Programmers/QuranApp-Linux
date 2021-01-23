/***
   * Project Name : Quran app
   * Description : an easy to use Quran Reading program in Qt5, supports translations in indian languages as well as quran audio streaming
   *               Specs :-  Multi-language , stream quran
   * Build-Dependencies : libcurl , qt5 libs, nlohmann json : https://github.com/nlohmann/json/ , libao-dev , libmpg123-dev
   * Developer : Nashid P , Member of Muslim Programmers Community
   * Project Status : stable
   *
   * Functions Description:-
   *
   * void help_menu() 
   *    Show help menu and exit
   *
   * int check_option(char *argv[], int x)
   *    check the arguments passed to the program and process it accordingly
   *
   * bool isSurah(char *arg)
   *    check if the given argument is surah , true if the argument contains only intigers
   *
   * int isInt(char *arg)
   *    check if the given argument is Integers , related to ayah processing
   *    return true if all chars except ':' are integers
   * 
   * void process_option(int argc, char *argv[], int x)
   *    pass arguments and process them accordingly , show either surah or ayah
***/

#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <cstdio>
#include "parser.hpp"
#include "audio_stream.hpp"
#include "window.hpp"

void help_menu();
int check_option(int argc, char *argv[], int x);
bool isSurah(char *arg);
bool isInt(char *arg);
void process_option(int argc, char *argv[], int x);

int main(int argc, char *argv[])
{
    QApplication qapp(argc, argv);
    Window *window;
    QPixmap pixmap("/opt/Qapp/qapp-splash.jpg");
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
    if(argc == 1)
    {
        window = new Window;
        splash.show();
        QTimer::singleShot(3000, &splash, &QWidget::close);
        window->show();
    }
    else
    {
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
        help_menu();
        exit(0);
    }
    else if(argv[x][2] == 's') // --s
    {
        std::string url_surah = "https://api.alquran.cloud/v1/surah/";
        if(isSurah(argv[x+1]))
        {
            if(!isInt(argv[x+1]))
            {
                std::cout << "\n\033[1;31m [surah] must be an integer , provided value = " << argv[x+1] << "\033[0m" << std::endl;
                exit(0);   
            }
            else
            {
                std::cout << "\033[1;32m Streaming .... \033[0m";
                url_surah.append(argv[x+1]);
                url_surah.append("/ar.alafasy");
                audio_stream newstream(url_surah);
                newstream.process_stream();
                std::cout << "\033[1;32m Stream ended \033[0m " << std::endl;
                exit(0);
            }
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
        if(arg[i] == ':')
            return false;
    }
    return true;
}

bool isInt(char *arg)
{
    int arglen = strlen(arg);
    int intlen = 0;
    bool symbol = false;
    for(int i=0;arg[i]!= '\0';i++)
    {
        if((arg[i] >= '0' && arg[i] <= '9') && arglen >= 1)
        {
            intlen++;
        }
        if(arg[i] == ':')
        {
            symbol = true;
            continue;
        }
    }
    if(intlen == arglen && symbol != true)
    {
        return true;
    }
    else if(symbol == true)
    {
        if(arglen != 2 && intlen == arglen-1)
            return true;
        else if(arglen == 2)
            return false;
    }
    return false;
}
