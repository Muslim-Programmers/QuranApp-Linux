#include <sqlite3.h>
#include "db_reader.hpp"
#include <fstream>

std::vector<std::string> Surah;

int QDBReader::callback(void *data, int argc, char *argv[], char *colname[])
{
    int i;
    for(i=1;i<argc;i++)
    {
        Surah.push_back(argv[i]);
    }
    return 0;
}

char *QDBReader::rep(char *str)
{
    char *dat = str;
    for (int i = 0; dat[i] != '\0'; i++)
    {
        if (dat[i] == '-')
            dat[i] = '_';
    }
    return dat;
}

std::vector<std::string> QDBReader::content(std::string table , std::string file)
{
    sqlite3 *QDB;
    int status = 0;
    status = sqlite3_open(file.c_str(), &QDB);
    std::string query = "SELECT * FROM " + table + ";";
    Surah.clear();
    sqlite3_exec(QDB, query.c_str(), callback, NULL, NULL);
    sqlite3_close(QDB);
    return Surah;
}

std::vector<std::string> QDBReader::metadata()
{
    std::vector<std::string> chapters;
    std::string data;
    std::ifstream inFile;
    inFile.open("/opt/qapp/database/Chapters.txt");
    while(std::getline(inFile, data))
    {
        chapters.push_back(data);
    }
    inFile.close();
    return chapters;
}

std::string QDBReader::getedition(std::string edition)
{
    if(edition == "English")
        return "/opt/qapp/database/quran_english.db";
    else if(edition == "Albanian")
        return "/opt/qapp/database/quran_albanian.db";
    else if(edition == "Azerbaijani")
        return "/opt/qapp/database/quran_azerbaijani.db";
    else if(edition == "Bengali")
        return "/opt/qapp/database/quran_bengali.db";
    else if(edition == "Czech")
        return "/opt/qapp/database/quran_czech.db";
    else if(edition == "Dutch")
        return "/opt/qapp/database/quran_dutch.db";
    else if(edition == "Farsi")
        return "/opt/qapp/database/quran_farsi.db";
    else if(edition == "French")
        return "/opt/qapp/database/quran_french.db";
    else if(edition == "German")
        return "/opt/qapp/database/quran_german.db";
    else if(edition == "Hausa")
        return "/opt/qapp/database/quran_hausa.db";
    else if(edition == "Hindi")
        return "/opt/qapp/database/quran_hindi.db";
    else if(edition == "Indonesian")
        return "/opt/qapp/database/quran_indonesian.db";
    else if(edition == "Japanese")
        return "/opt/qapp/database/quran_japanese.db";
    else if(edition == "Korean")
        return "/opt/qapp/database/quran_korean.db";
    else if(edition == "Kurdish")
        return "/opt/qapp/database/quran_kurdish.db";
    else if(edition == "Malayalam")
        return "/opt/qapp/database/quran_malayalam.db";
    else if(edition == "Maldivian")
        return "/opt/qapp/database/quran_maldivian.db";
    else if(edition == "Norwegian")
        return "/opt/qapp/database/quran_norwegian.db";
    else if(edition == "Polish")
        return "/opt/qapp/database/quran_polish.db";
    else if(edition == "Portuguese")
        return "/opt/qapp/database/quran_portuguese.db";
    else if(edition == "Sindhi")
        return "/opt/qapp/database/quran_sindhi.db";
    else if(edition == "Spanish")
        return "/opt/qapp/database/quran_spanish.db";
    else if(edition == "Swedish")
        return "/opt/qapp/database/quran_swedish.db";
    else if(edition == "Swahili")
        return "/opt/qapp/database/quran_swahili.db";
    else if(edition == "Romanian")
        return "/opt/qapp/database/quran_romanian.db";
    else if(edition == "Russian")
        return "/opt/qapp/database/quran_russian.db";
    else if(edition == "Tamil")
        return "/opt/qapp/database/quran_tamil.db";
    else if(edition == "Tajik")
        return "/opt/qapp/database/quran_tajik.db";
    else if(edition == "Thai")
        return "/opt/qapp/database/quran_thai.db";
    else if(edition == "Turkish")
        return "/opt/qapp/database/quran_turkish.db";
    else if(edition == "Uyghur")
        return "/opt/qapp/database/quran_uyghur.db";
    else if(edition == "Urdu")
        return "/opt/qapp/database/quran_urdu.db";
    else if(edition == "Uzbek")
        return "/opt/qapp/database/quran_uzbek.db";
    else
        return "/opt/qapp/database/quran.db";
    return NULL;
}
