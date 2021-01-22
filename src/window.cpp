#include <iostream>
#include "window.hpp"
#include "json.hpp"

using json = nlohmann::json;

Window::Window()
{
    QWidget *MainWidget = new QWidget;
    QVBoxLayout *MainLayout = new QVBoxLayout;
    MainLayout->addWidget(createComboBox());
    MainWidget->setLayout(MainLayout);
    setCentralWidget(MainWidget);
    setWindowTitle("Quran app");
}

QGroupBox *Window::createComboBox()
{  
    std::vector<std::string>data = getMetadata();
    QGroupBox *group = new QGroupBox;
    QHBoxLayout *layout = new QHBoxLayout;
    QComboBox *surah = new QComboBox;
    QComboBox *translation = new QComboBox;
    for(int i=0;i<114;i++)
    {
        surah->addItem(QString::number(i+1) + ". " + QString::fromStdString(data.at(i)));
    }
    layout->addWidget(translation);
    layout->addWidget(surah);
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