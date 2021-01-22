#ifndef __INTERFACE_H__
#define __INTERFACE_H__

class GUI : protected Parser 
{
public:
    GUI(const std::string link)
    {
        this->url = link;
    }
    void process_request(int requestType);
    void getayah(std::string buffer);
    void getsurah(std::string buffer);
    void list_surah(std::string buffer);
};

#endif