/***
   * Project Name : Quran app
   * Description : an easy to use Quran Reading program in Qt5, supports translations in indian languages as well as quran audio streaming
   *               Specs :-  Multi-language , stream quran
   * Build-Dependencies : libcurl , qt5 libs, nlohmann json : https://github.com/nlohmann/json/ , libao-dev , libmpg123-dev
   * Developer : Nashid P
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

#include <QtWidgets/QApplication>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <cstdio>
#include "parser.hpp"
#include "interface.hpp"
#include "audio_stream.hpp"
#include "window.hpp"
#include "translations.hpp"

void help_menu();
int check_option(int argc, char *argv[], int x);
bool isSurah(char *arg);
bool isInt(char *arg);
void process_option(int argc, char *argv[], int x);

int main(int argc, char *argv[])
{
    QApplication qapp(argc, argv);
    Window *window;
    if(argc == 1)
    {
        window = new Window();
        window->show();
        std::cout << "\n\033[1;31m No arguments provided , use '--h' for help menu \033[0m" << std::endl;
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
    std::cout << "\t --r (Read): " << std::endl;
    std::cout << "\t\t[arg]:- " <<std::endl;
    std::cout << "\t\t\t[surah] for a complete surah or [surah]:[ayah] for a single ayah " << std::endl;
    std::cout << "\t\t\t[surah] and [ayah] must be integer " << std::endl;
    std::cout << "\t\t\t[surah] [translation] or [surah]:[ayah] [translation] , for translated version " << std::endl;
    std::cout << "\t\t\tEg:- " << std::endl;
    std::cout << "\t\t\t   qapp --r 1 , return surah Al-Fatiha without any translation " << std::endl;
    std::cout << "\t\t\t   qapp --r 1:1 sahih , returns surah Al-Fatiha Verse 1 in saheeh international english translation" << std::endl;
    std::cout << "\t\t\t   qapp --r 1 sahih , returns surah Al-Fatiha \n" << std::endl;
    std::cout << "\t --s (Stream): " << std::endl;
    std::cout << "\t\t[arg]:- " << std::endl;
    std::cout << "\t\t\t[surah] stream complete surah " << std::endl;
    std::cout << "\t\t\tEg:- " << std::endl;
    std::cout << "\t\t\t   qapp --s 1 , stream surah Al-Fatiha , recitation of Mishary Alafasy \n" << std::endl;
    std::cout << "\t --h (Help): " << std::endl;
    std::cout << "\t\t show this help menu and exit \n" << std::endl;
    std::cout << "\t --t (Translations): " << std::endl;
    std::cout << "\t\t List of available translations \n" << std::endl;
    std::cout << "\t --l (List): " << std::endl;
    std::cout << "\t\t List all surah and their number , number should be used as [surah] arg for other options " << std::endl;
}

int check_option(int argc, char *argv[], int x)
{
    if(argv[x][2] == 'r' && argc >= 3) // --r
    {    
        process_option(argc,argv,x);
    }
    else if(argv[x][2] == 'h') // --h
    {
        help_menu();
        exit(0);
    }
    else if(argv[x][2] == 't') // --t
    {
        tr_help();
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
    else if(argv[x][2] == 'l')
    {
        std::string meta = "https://api.alquran.cloud/v1/meta";
        GUI metadata(meta);
        metadata.process_request(2);
        exit(0);
    } 
    else 
    {
        std::cout << "\033[1;31m Invalid option provided or insufficient arguements , use --h to show help menu \033[0m" << std::endl; 
        exit(0);
    }
    return x;
}

void process_option(int argc, char *argv[], int x)
{
    std::string url_ayah = "https://api.alquran.cloud/v1/ayah/";
    std::string url_surah = "https://api.alquran.cloud/v1/surah/";
    std::string edition = "quran-simple-enhanced";

    if(isSurah(argv[x+1]))
    {
        if(!isInt(argv[x+1]))
        {
            std::cout << "\n [surah] must be an integer , provided value = " << argv[x+1] << std::endl;
            exit(0);
        } 
        else 
        {
            if(argc == 4)
            {
                edition = return_edition(static_cast<std::string>(argv[x+2]));
            }
            url_surah.append(argv[x+1]);
            url_surah.append("/"+edition);
            GUI Surah_GUI(url_surah);
            Surah_GUI.process_request(1);
        }
    } 
    else 
    {
        if(!isInt(argv[x+1]))
        {
            std::cout << "\n\033[1;31m [surah] and [ayah] must be an integer , provided value = " << argv[x+1] << "\033[0m" << std::endl;
            exit(0);
        } 
        else 
        {
            if(argc == 4)
            {
                edition = return_edition(static_cast<std::string>(argv[x+2]));
            }
            url_ayah.append(argv[x+1]);
            url_ayah.append("/"+edition);
            GUI Ayah_GUI(url_ayah);
            Ayah_GUI.process_request(0);
        }
    }
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
