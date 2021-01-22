/***
   * void process_request(int requestType)
   *    show ayah or surah in GUI mode , if requestType = 0 , show ayah
   *    else if requestType = 1 , show surah
   *
   * void getayah(std::string str_buffer)
   *    accept GET request result of ayah and display it in GUI mode
   *
   * void getsurah(std::string str_buffer)
   *    accept GET request result of surah and display it in GUI mode
   *
***/

#include <QtWidgets/QApplication>
#include <QtCore/QString>
#include <QtCore/QSize>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTextEdit>
#include <QtGui/QTextCursor>
#include <QtGui/QTextBlockFormat>
#include <vector>
#include <string>
#include <iostream>
#include "parser.hpp"
#include "interface.hpp"
#include "json.hpp"

using json = nlohmann::json;

void GUI::process_request(int requestType)
{
    if(requestType == 0)
    {
        this->getayah(this->curl_process());
    } else if(requestType == 1) {
        this->getsurah(this->curl_process());
    }
    else if(requestType == 2)
    {
        this->list_surah(this->curl_process());
    }
}

void GUI::getayah(std::string buffer)
{
    try
    {
        json parse_ayah = json::parse(buffer);
        QTextEdit *gui = new QTextEdit();
        // surah Name and Name meaning
        gui->setText(QString::fromStdString(parse_ayah["data"]["surah"]["name"].get<std::string>()));
        QTextCursor cursor = gui->textCursor();
        QTextBlockFormat textBlockFormat = cursor.blockFormat();
        textBlockFormat.setAlignment(Qt::AlignCenter);
        cursor.mergeBlockFormat(textBlockFormat);
        gui->append("(" + QString::fromStdString(parse_ayah["data"]["surah"]["englishNameTranslation"].get<std::string>()) + ")" + "\n");
        gui->append(QString::fromStdString(parse_ayah["data"]["text"].get<std::string>()));
        gui->selectAll();
        gui->setFontPointSize(16);
        gui->setTextCursor(cursor);
        gui->setReadOnly(true);
        QWidget *widget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(gui);
        widget->setLayout(layout);
        widget->setMinimumHeight(558);
        widget->setMinimumWidth(558);
        widget->setWindowTitle("Quran app");
        widget->show();
    } catch( nlohmann::detail::type_error &err)
    {
        std::cout << "\033[1;31m An error occured , make sure you entered valid surah and ayah \033[0m" << std::endl;
        exit(0);
    }
}

void GUI::getsurah(std::string buffer)
{
    try
    {
        json parse_surah = json::parse(buffer);
        int ayahs = parse_surah["data"]["numberOfAyahs"]; // Number of ayahs
        std::vector<QString>parsed_data;
        int count = 1;
        for(int i=0;i<ayahs;i++)
        {
            // push each ayah into vector
            parsed_data.push_back(QString::fromStdString(parse_surah["data"]["ayahs"][i]["text"].get<std::string>())); 
        }
        QTextEdit *gui = new QTextEdit();
        // surah Name and Name Meaning
        gui->setText(QString::fromStdString(parse_surah["data"]["name"].get<std::string>()) + "\n");
        QTextCursor cursor = gui->textCursor();
        QTextBlockFormat textBlockFormat = cursor.blockFormat();
        textBlockFormat.setAlignment(Qt::AlignCenter);
        cursor.mergeBlockFormat(textBlockFormat);
        for(QString ayah : parsed_data)
        {
            QString data =  QString::fromStdString(std::to_string(count)) + ". " + ayah;
            gui->append(data + "\n");
            count++;
        }
        gui->append("Edition : " + QString::fromStdString(parse_surah["data"]["edition"]["name"]));
        gui->selectAll();
        gui->setFontPointSize(16);
        gui->setTextCursor(cursor);
        gui->setReadOnly(true);
        QWidget *widget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(gui);
        widget->setLayout(layout);
        widget->setMinimumHeight(558);
        widget->setMinimumWidth(558);
        widget->setWindowTitle("Quran app");
        widget->show();
    } catch( nlohmann::detail::type_error &err)
    {
        std::cout << "\033[1;31m An error occured , make sure you entered valid surah \033[0m" << std::endl;
        exit(0);
    }
}

void GUI::list_surah(std::string buffer)
{
    json metadata = json::parse(buffer);
    std::cout << " List of Surahs :- \n";
    for(int i=0;i<114;i++)
    {
        std::cout << "\033[1;34m Surah Number           : \033[1;31m" << metadata["data"]["surahs"]["references"][i]["number"] << std::endl;
        std::cout << "\033[1;34m Surah Name (Arabic)    : \033[1;32m" << metadata["data"]["surahs"]["references"][i]["name"] << std::endl;
        std::cout << "\033[1;34m Surah Name (English)   : " << metadata["data"]["surahs"]["references"][i]["englishName"] << std::endl;
        std::cout << " Surah Name Translation : " << metadata["data"]["surahs"]["references"][i]["englishNameTranslation"] << std::endl;
        std::cout << " Number of Ayahs        : " << metadata["data"]["surahs"]["references"][i]["numberOfAyahs"] << std::endl;
        std::cout << " Revelation Type        : " << metadata["data"]["surahs"]["references"][i]["revelationType"] << "\n" << std::endl;
    }
    std::cout << "\033[0m";
}
