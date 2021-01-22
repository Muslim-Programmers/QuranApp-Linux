/***
   * void tr_help() { defined in headerfile translations.h }
   *    show available translations
   *
   * std::string return_edition(std::string arg)
   *    check the arg and return the appropriate translation , if arg is invalid return " " which is pure quran without translation
   *
***/
  
#pragma once
#ifndef __TRANSLATIONS_H__
#define __TRANSLATIONS_H__

#include <iostream>
#include <string>

void tr_help()
{
    std::cout << " Supported translations :- \n" << std::endl;
    std::cout << " Example Usage : qapp --o 1:1 Hindi-1 " << std::endl;
    std::cout << "\t English :-" << std::endl;
    std::cout << "\t\t Translated by Saheeh International : English " << std::endl;
    std::cout << "\t Hindi :- " << std::endl;
    std::cout << "\t\t Translated by Suhel Farooq Khan and Saifur Rahman Nadwi : Hindi" << std::endl;
    std::cout << "\t Malayalam :- " << std::endl;
    std::cout << "\t\t Translated by Cheriyamundam Abdul Hameed and Kunhi Mohammed Parappoor : Malayalam" << std::endl;
    std::cout << "\t Tamil :- " << std::endl;
    std::cout << "\t\t Translated by Jan Turst Foundation : Tamil" << std::endl;
    std::cout << "\t Turkish :- " << std::endl;
    std::cout << "\t\t Translated by Suleyman Ates : Turkish" << std::endl;
    std::cout << "\t Urdu :- " << std::endl;
    std::cout << "\t\t Translated by Ahmed Ali : Urdu" << std::endl;  
}

std::string return_edition(std::string arg)
{
    if(arg == "English")
    {
        return "en.sahih";
    }
    else if(arg == "Hindi")
    {
        return "hi.hindi";
    }
    else if(arg == "Malayalam")
    {
        return "ml.abdulhameed";
    }
    else if(arg == "Tamil")
    {
        return "ta.tamil";
    }
    else if(arg == "Turkish")
    {
        return "tr.ates";
    }
    else if(arg == "Urdu")
    {
        return "ur.ahmedali";
    }
    else
    {
        std::cout << "\033[1;31m Invalid Language please check available languages using option '--t' \033[0m" << std::endl;
        std::cout << " Choosing default language arabic " << std::endl;
    }
    return " ";
}

#endif
