#pragma once
#ifndef DB_READER_HPP
#define DB_READER_HPP

#include <vector>
#include <iostream>
#include <string>

class QDBReader{
public:
    static int callback(void*, int, char**, char**);
    std::vector<std::string> content(std::string , std::string);
    std::vector<std::string> metadata();
    std::string getedition(std::string);
    char *replaceStr(char *str);
};

#endif // DB_READER_HPP
