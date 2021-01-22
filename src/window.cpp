#include <iostream>
#include <string>
#include "window.hpp"
#include "json.hpp"

using json = nlohmann::json;

Window::Window()
{
    QWidget *MainWidget = new QWidget;
    QVBoxLayout *MainLayout = new QVBoxLayout;
    createMenu();
    setMenuBar(menuBar);
    MainLayout->addWidget(createComboBox());
    MainLayout->addWidget(createTextBox());
    MainWidget->setLayout(MainLayout);
    setCentralWidget(MainWidget);
    setWindowTitle("Quran app");
    setMinimumWidth(800);
    setMinimumHeight(600);
    connect(surah, SIGNAL(currentTextChanged(QString)), this, SLOT(showSurah()));
    connect(translation, SIGNAL(currentTextChanged(QString)), this, SLOT(showTranslation()));
}

void Window::createMenu()
{
    menuBar = new QMenuBar;
    Menu = new QMenu(tr("&Menu"), this);
    about = Menu->addAction(tr("A&bout"));
    menuBar->addMenu(Menu);
    connect(about, SIGNAL(triggered()), this, SLOT(showAbout()));
}

QGroupBox *Window::createComboBox()
{  
    std::vector<std::string>data = getMetadata();
    QGroupBox *group = new QGroupBox;
    QHBoxLayout *layout = new QHBoxLayout;
    surah = new QComboBox;
    translation = new QComboBox;
    for(int i=0;i<114;i++)
    {
        surah->addItem(QString::number(i+1) + ". " + QString::fromStdString(data.at(i)));
    }
    translation->addItem("English");
    translation->addItem("Hindi");
    translation->addItem("Malayalam");
    translation->addItem("Turkish");
    translation->addItem("Tamil");
    translation->addItem("Urdu");
    layout->addWidget(translation);
    layout->addWidget(surah);
    group->setLayout(layout);
    return group;
}

QGroupBox *Window::createTextBox()
{
    std::string surah_url = "https://api.alquran.cloud/v1/surah/";
    std::string translation_url = "https://api.alquran.cloud/v1/surah/";
    surah_url.append(std::to_string(surah_number) + "/");
    surah_url.append("quran-simple-enhanced");
    translation_url.append(std::to_string(surah_number) + "/");
    translation_url.append(edition);
    QGroupBox *group = new QGroupBox;
    QHBoxLayout *layout = new QHBoxLayout;
    show_surah = new QTextEdit;
    show_translation = new QTextEdit;
    getSurah(surah_url);
    getTranslation(translation_url);
    layout->addWidget(show_translation);
    layout->addWidget(show_surah);
    group->setLayout(layout);
    return group;
}

std::vector<std::string> Window::getMetadata()
{
    std::vector<std::string>data;
    this->url = "https://api.alquran.cloud/v1/meta";
    json metadata = json::parse(this->curl_process());
    for(int i=0;i<114;i++)
    {
        data.push_back(metadata["data"]["surahs"]["references"][i]["englishName"]);
    }
    return data;
}

void Window::getSurah(std::string surah_url)
{
    
    this->url = surah_url;
    QVector<QString>parsed_data;
    json parse_surah = json::parse(curl_process());
    int ayahs = parse_surah["data"]["numberOfAyahs"];
    int count = 1;
    show_surah->setText(QString::fromStdString(parse_surah["data"]["name"].get<std::string>()));
    QTextCursor cursor = show_surah->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignCenter);
    cursor.mergeBlockFormat(textBlockFormat);
    for(int i=0;i<ayahs;i++)
    {
        parsed_data.push_back(QString::fromStdString(parse_surah["data"]["ayahs"][i]["text"].get<std::string>()));
    }
    for(QString ayah : parsed_data)
    {
        QString data =  QString::fromStdString(std::to_string(count)) + ". " + ayah;
        show_surah->append(data + "\n");
        count++;
    }
    show_surah->selectAll();
    show_surah->setFontPointSize(16);
    show_surah->setTextCursor(cursor);
    show_surah->setReadOnly(true);
}

void Window::getTranslation(std::string translation_url)
{
    this->url = translation_url;
    QVector<QString>parsed_data;
    json parse_surah = json::parse(curl_process());
    int ayahs = parse_surah["data"]["numberOfAyahs"];
    int count = 1;
    show_translation->setText(QString::fromStdString(parse_surah["data"]["englishName"].get<std::string>()));
    QTextCursor cursor = show_translation->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignCenter);
    cursor.mergeBlockFormat(textBlockFormat);
    for(int i=0;i<ayahs;i++)
    {
        parsed_data.push_back(QString::fromStdString(parse_surah["data"]["ayahs"][i]["text"].get<std::string>()));
    }
    for(QString ayah : parsed_data)
    {
        QString data =  QString::fromStdString(std::to_string(count)) + ". " + ayah;
        show_translation->append(data + "\n");
        count++;
    }
    show_translation->selectAll();
    show_translation->setFontPointSize(16);
    show_translation->setTextCursor(cursor);
    show_translation->setReadOnly(true);
}

void Window::showSurah()
{
    std::string surah_url = "https://api.alquran.cloud/v1/surah/";
    std::string translation_url = surah_url;
    std::vector<std::string>data;
    char *token =  strtok((char *)surah->currentText().toStdString().c_str(), ".");
    while(token != NULL)
    {   
        data.push_back(token);
        token = strtok(NULL, ".");
    }
    surah_number = std::stoi(data.at(0));
    surah_url.append(std::to_string(surah_number) + "/");
    surah_url.append("quran-simple-enhanced");
    edition = getEdition(translation->currentText().toStdString());
    translation_url.append(std::to_string(surah_number) + "/");
    translation_url.append(edition);
    getSurah(surah_url);
    getTranslation(translation_url);
}

void Window::showTranslation()
{
    std::string translation_url = "https://api.alquran.cloud/v1/surah/";
    edition = getEdition(translation->currentText().toStdString());
    translation_url.append(std::to_string(surah_number) + "/");
    translation_url.append(edition);
    getTranslation(translation_url);
}

std::string Window::getEdition(std::string identifier)
{
    if(identifier == "English")
    {
        return "en.sahih";
    }
    else if(identifier == "Hindi")
    {
        return "hi.hindi";
    }
    else if(identifier == "Malayalam")
    {
        return "ml.abdulhameed";
    }
    else if(identifier == "Tamil")
    {
        return "ta.tamil";
    }
    else if(identifier == "Turkish")
    {
        return "tr.ates";
    }
    else if(identifier == "Urdu")
    {
        return "ur.ahmedali";
    }
}

void Window::showAbout()
{
    QWidget *AboutWindow = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    QTextEdit *about_section = new QTextEdit;
    about_section->setText(" Developed by Muslim Programmers Community © 2021.\n");
    about_section->append(" Discord : discord.gg/7cnWVc8qgb \n");
    about_section->append(" Instagram @muslimpgmrs \n");
    about_section->append(" Github : https://github.com/Muslim-Programmers/ ");
    QTextCursor cursor = about_section->textCursor();
    about_section->selectAll();
    about_section->setFontPointSize(14);
    about_section->setReadOnly(true);
    about_section->setTextCursor(cursor);
    layout->addWidget(about_section);
    AboutWindow->setLayout(layout);
    AboutWindow->setMinimumSize(602,443);
    AboutWindow->show();
}