/***
   * void tr_help() { defined in headerfile translations.h }
   *    show available translations
   *
   * std::string request_edition(std::string arg)
   *    check the arg and return the appropriate translation , if arg is invalid return " " which is pure quran without translation
   *
***/
  

#ifndef __TRANSLATIONS_H__
#define __TRANSLATIONS_H__

#include <iostream>
#include <string>

void tr_help()
{
    std::cout << " Supported translations :- \n" << std::endl;
    std::cout << " Example Usage : qapp --o 1:1 hindi-1 " << std::endl;
    std::cout << "\t English :-" << std::endl;
    std::cout << "\t\t Translated by Saheeh International = sahih " << std::endl;
    std::cout << "\t Hindi :- " << std::endl;
    std::cout << "\t\t Translated by Suhel Farooq Khan and Saifur Rahman Nadwi : hindi-1" << std::endl;
    std::cout << "\t\t Translated by Muhammad Farooq Khan and Muhammad Ahmed : hindi-2" << std::endl;
    std::cout << "\t Malayalam :- " << std::endl;
    std::cout << "\t\t Translated by Cheriyamundam Abdul Hameed and Kunhi Mohammed Parappoor : malayalam" << std::endl;
    std::cout << "\t Tamil :- " << std::endl;
    std::cout << "\t\t Translated by Jan Turst Foundation : tamil" << std::endl;
    std::cout << "\t Turkish :- " << std::endl;
    std::cout << "\t\t Translated by Suleyman Ates : turkish" << std::endl;
    std::cout << "\t Urdu :- " << std::endl;
    std::cout << "\t\t Translated by Ahmed Ali : urdu-1" << std::endl;
    std::cout << "\t\t Translated by Fateh Muhammad Jalandhry : urdu-2" << std::endl;
    std::cout << "\t\t Translated by Syed Zeeshan Haider Jawadi : urdu-3" << std::endl;
    std::cout << "\t\t Translated by Ahmed Raza Khan : urdu-4" << std::endl;
    std::cout << "\t\t Translated by Tahir ul Qadri : urdu-5" << std::endl;
    std::cout << "\t\t Translated by Muhammad Junagarhi : urdu-6" << std::endl;
    std::cout << "\t\t Translated by Abul A'ala Maududi : urdu-7" << std::endl;
     
}

std::string return_edition(std::string arg)
{
    if(arg == "sahih")
    {
        return "en.sahih";
    }
    else if(arg == "hindi-1")
    {
        return "hi.hindi";
    }
    else if(arg == "hindi-2")
    {
        return "hi.farooq";
    }
    else if(arg == "malayalam")
    {
        return "ml.abdulhameed";
    }
    else if(arg == "tamil")
    {
        return "ta.tamil";
    }
    else if(arg == "turkish")
    {
        return "tr.ates";
    }
    else if(arg == "urdu-1")
    {
        return "ur.ahmedali";
    }
    else if(arg == "urdu-2")
    {
        return "ur.jalandhry";
    }
    else if(arg == "urdu-3")
    {
        return "ur.jawadi";
    }
    else if(arg == "urdu-4")
    {
        return "ur.kanzuliman";
    }
    else if(arg == "urdu-5")
    {
        return "ur.qadri";
    }
    else if(arg == "urdu-6")
    {
        return "ur.junargarhi";
    }
    else if(arg == "urdu-7")
    {
        return "ur.maududi";
    }
    else
    {
        std::cout << "\033[1;31m Invalid Language please check available languages using option '--t' \033[0m" << std::endl;
        std::cout << " Choosing default language arabic " << std::endl;
    }
    return " ";
}

#endif
