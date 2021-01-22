#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>

class Parser {
protected:
    std::string url;
    std::string curl_process();
    static std::size_t WriteMemoryCallback(char *in, std::size_t size, std::size_t nmemb, std::string *out);
};

#endif
