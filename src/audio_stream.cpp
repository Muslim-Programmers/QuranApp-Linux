/**
 * std::size_t play_stream(params)
 *      play audio from curl buffer
 * 
 * void process_stream()
 *      load audio url and save it to buffer
 * 
 * std::string audio_url(int)
 *      return mp3 url 
**/

#include <iostream>
#include <curl/curl.h>
#include <mpg123.h>
#include <ao/ao.h>
#include <vector>
#include "audio_stream.hpp"
#include "json/json.hpp"
#include "curl_parser.hpp"

#define BITS 8

using json = nlohmann::json;

mpg123_handle *mh = NULL;
ao_device *dev = NULL;

std::size_t audio_stream::play_stream(void *buffer, std::size_t size, std::size_t nmemb, void *userp)
{
    int err;
    off_t frame_offset;
    char *audio;
    std::size_t done;
    ao_sample_format format;
    int channels, encoding;
    long rate;
    mpg123_feed(mh, (const unsigned char*) buffer, size *nmemb);
    do {
        err = mpg123_decode_frame(mh, &frame_offset, (unsigned char**)&audio, &done);
        switch(err)
        {
            case MPG123_NEW_FORMAT:
                mpg123_getformat(mh, &rate, &channels, &encoding);
                format.bits = mpg123_encsize(encoding) * BITS;
                format.rate = rate;
                format.channels = channels;
                format.byte_format = AO_FMT_NATIVE;
                format.matrix = 0;
                dev = ao_open_live(ao_default_driver_id(), &format, NULL);
                break;
            case MPG123_OK:
                ao_play(dev, audio, done);
                break;
            case MPG123_NEED_MORE:
                break;
            default:
                break;                
        }
    } while(done > 0);

    return size * nmemb;
}

void audio_stream::process_stream()
{
    try
    {
        ao_initialize();
        mpg123_init();
        mh = mpg123_new(NULL, NULL);
        mpg123_open_feed(mh);
        json metadata = json::parse(curl_process());
        int numberOfayahs = metadata["data"]["numberOfAyahs"];
        int surah_number = metadata["data"]["number"];
        std::cout << "\n\033[1;34m Stream Info:- " << std::endl;
        std::cout << " Surah Name : " << metadata["data"]["englishName"] << std::endl;
        std::cout << " Surah Number : " << surah_number << std::endl;
        std::cout << " Number of Ayahs : " << numberOfayahs << std::endl;
        std::cout << " Reciter : " << metadata["data"]["edition"]["name"] << " (" << metadata["data"]["edition"]["englishName"] << " ) \033[0m" << std::endl;
        CURL *curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, play_stream);
        curl_easy_setopt(curl, CURLOPT_URL, audio_url(surah_number).c_str());
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        ao_close(dev);
        ao_shutdown();
    } catch( nlohmann::detail::type_error &err)
    {
        std::cout << "\033[1;31m An error occured , make sure you entered valid surah \033[0m" << std::endl;
    }
}

std::string audio_stream::audio_url(int surah_number)
{
    std::string stream_url = "https://server8.mp3quran.net/afs/";
    if(surah_number >= 1 && surah_number <=9)
    {
        stream_url.append("00");
        stream_url.append(std::to_string(surah_number));
        stream_url.append(".mp3");
    }
    else if(surah_number >= 10 && surah_number <= 99)
    {
        stream_url.append("0");
        stream_url.append(std::to_string(surah_number));
        stream_url.append(".mp3");
    }
    else if(surah_number >= 100 && surah_number <= 114)
    {
        stream_url.append(std::to_string(surah_number));
        stream_url.append(".mp3");
    }
    return stream_url;
}