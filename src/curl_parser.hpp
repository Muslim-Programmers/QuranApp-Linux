#pragma once
#ifndef __CURL_PARSER_H__
#define __CURL_PARSER_H__

#include <string>

class CURLParser {
protected:
    std::string url;
    std::string curl_process();
    static std::size_t WriteMemoryCallback(char *in, std::size_t size, std::size_t nmemb, std::string *out);
};

#endif
