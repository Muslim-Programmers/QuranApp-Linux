#include <iostream>
#include <string>
#include <QFile>
#include <QTextStream>
#include "window.hpp"
#include "json/json.hpp"

using json = nlohmann::json;

const std::string languages [] = {
    "Albanian",
    "Azerbaijani",
    "Bengali",
    "Czech",
    "Dutch",
    "English",
    "Farsi",
    "French",
    "German",
    "Hausa",
    "Hindi",
    "Indonesian",
    "Japanese",
    "Korean",
    "Kurdish",
    "Malayalam",
    "Maldivian",
    "Norwegian",
    "Polish",
    "Portuguese",
    "Sindhi",
    "Spanish",
    "Swedish",
    "Swahili",
    "Romanian",
    "Russian",
    "Tamil",
    "Tajik",
    "Thai",
    "Turkish",
    "Uyghur",
    "Urdu",
    "Uzbek"
};

Window::Window()
{
    QWidget *MainWidget = new QWidget;         // Main Widget
    QVBoxLayout *MainLayout = new QVBoxLayout; // Vertical Layout
    createMenu();                              // Call createMenu() to configure MenuBar
    setMenuBar(menuBar);                       // set MenuBar
    MainLayout->addWidget(createComboBox());   // set ComboBox
    MainLayout->addWidget(createTextBox());    // set TextBox
    MainWidget->setLayout(MainLayout);         // set Layout
    setCentralWidget(MainWidget);              // set Main Widget
    setWindowTitle("Quran App");               // set Window Title
    setMinimumWidth(800);                      // set Minimum Width of Window
    setMinimumHeight(600);                     // set Minimum Height of Window
    connect(surah, SIGNAL(currentTextChanged(QString)), this, SLOT(showSurah())); // Watch Changes in ComboBox surah
    connect(translation, SIGNAL(currentTextChanged(QString)), this, SLOT(showTranslation())); // Watch Changes in ComboBox translation
}

void Window::createMenu()
{
    // Function to Configure MenuBar

    menuBar = new QMenuBar;
    Menu = new QMenu(tr("Menu"), this);
    darkmode = Menu->addAction(tr("Dark Mode")); // Add Dark Mode Menu Entry
    prayertimes = Menu->addAction(tr("Prayer Times"));
    about = Menu->addAction(tr("About"));        // Add About Menu Entry
    darkmode->setCheckable(true);
    menuBar->addMenu(Menu);
    connect(about, SIGNAL(triggered()), this, SLOT(showAbout())); 
    connect(darkmode, SIGNAL(triggered()), this, SLOT(setDarkMode()));
    connect(prayertimes, SIGNAL(triggered()), this, SLOT(showPrayerTimes()));
}

QGroupBox *Window::createComboBox()
{
    // Function to Configure ComboBox

    std::vector<std::string>data = getMetadata();
    QGroupBox *group = new QGroupBox;      // GroupBox ( To Group Widgets )
    QHBoxLayout *layout = new QHBoxLayout; // Horizontal Layout
    surah = new QComboBox;      
    translation = new QComboBox;
    if(data.empty())
    {
        surah->addItem("No Internet"); // Add text to ComboBox
    }
    else
    {
        for(int i=0;i<114;i++)
        {
            surah->addItem(QString::number(i+1) + ". " + QString::fromStdString(data.at(i)));
        }
    }
    for(auto element : languages)
    {
        translation->addItem(QString::fromStdString(element));
    }
    layout->addWidget(translation);
    layout->addWidget(surah);
    group->setLayout(layout);
    return group;
}

QGroupBox *Window::createTextBox()
{
    // Function to configure Text Box

    std::string surah_url = "https://api.alquran.cloud/v1/surah/";
    std::string translation_url = "https://api.alquran.cloud/v1/surah/";
    surah_url.append(std::to_string(surah_number) + "/");
    surah_url.append("quran-simple-enhanced");
    translation_url.append(std::to_string(surah_number) + "/");
    translation_url.append(edition);
    translation->setCurrentIndex(5);
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
    // Function to retrieve the surah Names

    std::vector<std::string>data;
    this->url = "https://api.alquran.cloud/v1/meta";
    try {
        json metadata = json::parse(this->curl_process()); // Parse the JSON data
        for(int i=0;i<114;i++)
        {
            data.push_back(metadata["data"]["surahs"]["references"][i]["englishName"]); 
        }
    } catch(nlohmann::json::type_error &err) {

        // caught if any errors encountered during JSON parsing

        QMessageBox::critical(this, "Error", "No Internet Connection , Please enable internet and restart the Application");
    }
    return data;
}

void Window::getSurah(std::string surah_url)
{
    
    this->url = surah_url;
    QVector<QString>parsed_data;
    try {
        json parse_surah = json::parse(curl_process());
        int ayahs = parse_surah["data"]["numberOfAyahs"];
        int count = 1;
        show_surah->setText(QString::fromStdString(parse_surah["data"]["name"].get<std::string>()));
        QTextCursor cursor = show_surah->textCursor();
        QTextBlockFormat textBlockFormat = cursor.blockFormat();
        textBlockFormat.setAlignment(Qt::AlignCenter);              // Align Text to Center
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
        show_surah->setFontPointSize(16); // set Font Size
        show_surah->setTextCursor(cursor);
        show_surah->setReadOnly(true);    // set Text Box Read Only
    } catch(nlohmann::json::type_error &err) {
        show_surah->setText(" Please Enable Internet to use this application ");
    }
}

void Window::getTranslation(std::string translation_url)
{
    this->url = translation_url;
    QVector<QString>parsed_data;
    try {

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
    } catch(nlohmann::json::type_error &err) {
        show_translation->setText(" Please Enable Internet to use this application ");
    }
}

void Window::showSurah()
{
    std::string surah_url = "https://api.alquran.cloud/v1/surah/";
    std::string translation_url = surah_url;
    std::vector<std::string>data;
    surah_number = surah->currentIndex() + 1;
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
    if(identifier == "Albanian")
    {
        return "sq.ahmeti";
    }
    else if(identifier == "Azerbaijani")
    {
        return "az.mammadaliyev";
    }
    else if(identifier == "Bengali")
    {
        return "bn.bengali";
    }
    else if(identifier == "Czech")
    {
        return "cs.hrbek";
    }
    else if(identifier == "Dutch")
    {
        return "nl.keyzer";
    }
    else if(identifier == "English")
    {
        return "en.sahih";
    }
    else if(identifier == "Farsi")
    {
        return "fa.ayati";
    }
    else if(identifier == "French")
    {
        return "fr.hamidullah";
    }
    else if(identifier == "German")
    {
        return "de.aburida";
    }
    else if(identifier == "Hausa")
    {
        return "ha.gumi";
    } 
    else if(identifier == "Hindi")
    {
        return "hi.hindi";
    }
    else if(identifier == "Indonesian")
    {
        return "id.indonesian";
    }
    else if(identifier == "Japanese")
    {
        return "ja.japanese";
    }
    else if(identifier == "Korean")
    {
        return "ko.korean";
    }
    else if(identifier == "Kurdish")
    {
        return "ku.asan";
    }
    else if(identifier == "Malayalam")
    {
        return "ml.abdulhameed";
    }
    else if(identifier == "Maldivian")
    {
        return "dv.divehi";   
    }
    else if(identifier == "Norwegian")
    {
        return "no.berg";
    }
    else if(identifier == "Polish")
    {
        return "pl.bielawskiego";
    }
    else if(identifier == "Portuguese")
    {
        return "pt.elhayek";
    }
    else if(identifier == "Sindhi")
    {
        return "sd.amroti";
    }
    else if(identifier == "Spanish")
    {
        return "es.cortes";
    }
    else if(identifier == "Swedish")
    {
        return "sv.bernstorm";
    }
    else if(identifier == "Swahili")
    {
        return "sw.barwani";
    }

    else if(identifier == "Romanian")
    {
        return "ro.grigore";
    }
    else if(identifier == "Russian")
    {
        return "ru.kuliev";
    }
    else if(identifier == "Tamil")
    {
        return "ta.tamil";
    }
    else if(identifier == "Tajik")
    {
        return "tg.ayati";
    }
    else if(identifier == "Thai")
    {
        return "th.thai";
    }
    else if(identifier == "Turkish")
    {
        return "tr.ates";
    }
    else if(identifier == "Uyghur")
    {
        return "ug.saleh";
    }
    else if(identifier == "Urdu")
    {
        return "ur.ahmedali";
    }
    else if (identifier == "Uzbek")
    {
        return "uz.sodik";
    }
}

void Window::showAbout()
{
    QWidget *AboutWindow = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    QPixmap pixmap("/opt/qapp/resources/qapp-198x198.png");
    QFile f("/opt/qapp/resources/qdarkstyle/style.qss");
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    QFont hfont("Arial", 15, QFont::Bold);
    QFont nfont("Arial", 13, QFont::AnyStyle);
    QFont ffont("Arial", 12, QFont::AnyStyle);
    ffont.setItalic(true);
    QLabel *icon = new QLabel;
    QLabel *header = new QLabel("A Project by Muslim Programmers Community");
    QLabel *Discord = new QLabel("Discord : discord.gg/7cnWVc8qgb");
    QLabel *Instagram = new QLabel("Instagram : @muslimpgmrs");
    QLabel *Contributers = new QLabel("Contributers : Nashid , Jonas , Bilal");
    QLabel *footer = new QLabel("www.muslimprogrammers.com");
    icon->setPixmap(pixmap);
    icon->setAlignment(Qt::AlignCenter);
    icon->setGeometry(QRect(312, 454, 21, 20));
    header->setAlignment(Qt::AlignCenter);
    header->setFont(hfont);
    header->setGeometry(QRect(312, 454, 21, 20));
    Discord->setFont(nfont);
    Discord->setGeometry(QRect(300, 400, 50, 50));
    Instagram->setFont(nfont);
    Discord->setTextInteractionFlags(Qt::TextSelectableByMouse);
    Discord->setAlignment(Qt::AlignCenter);
    Instagram->setAlignment(Qt::AlignCenter);
    Instagram->setTextInteractionFlags(Qt::TextSelectableByMouse);
    Contributers->setFont(nfont);
    Contributers->setAlignment(Qt::AlignCenter);
    footer->setFont(ffont);
    footer->setAlignment(Qt::AlignCenter);
    footer->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(icon);
    layout->addWidget(header);
    layout->addWidget(Discord);
    layout->addWidget(Instagram);
    layout->addWidget(Contributers);
    layout->addWidget(footer);
    AboutWindow->setLayout(layout);
    AboutWindow->setMinimumSize(602,443);
    AboutWindow->setWindowTitle("About");
    if(dark_mode_enabled)
        AboutWindow->setStyleSheet(ts.readAll());
    else
        AboutWindow->setStyleSheet("");
    AboutWindow->show();
}

void Window::setDarkMode()
{
    QFile f("/opt/qapp/resources/qdarkstyle/style.qss"); 
    f.open(QFile::ReadOnly | QFile::Text); // open window stylesheet
    QTextStream ts(&f); 
    if(dark_mode_enabled)
    {
        setStyleSheet("");
        dark_mode_enabled = false;
    }
    else
    {
        setStyleSheet(ts.readAll());
        dark_mode_enabled = true;
    }
}

void Window::showPrayerTimes()
{
    PayerTimeWidget = new QWidget;
    QGridLayout *Layout = new QGridLayout;
    QFile f("/opt/qapp/resources/qdarkstyle/style.qss");
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    QFont timeFont("Arial", 18, QFont::Bold);
    QFont textFont("Arial", 15, QFont::Bold);
    QDate today = QDate::currentDate();
    QLabel *DateToday = new QLabel(today.toString("dd MMM yyyy"));
    timer = new QTimer;
    display = new QLabel;
    imsak = new QLabel("Imsak: ");
    fajr = new QLabel("Fajr: ");
    sunrise = new QLabel("Sunrise: ");
    zuhr = new QLabel("Zuhr: ");
    asr = new QLabel("Asr: ");
    maghrib = new QLabel("Maghrib: ");
    sunset = new QLabel("Sunset: ");
    isha = new QLabel("Isha: ");
    midnight = new QLabel("Midnight: ");
    Country = new QLineEdit;
    City = new QLineEdit;
    Show = new QPushButton("Show Time");
    Country->setPlaceholderText("Country");
    City->setPlaceholderText("City");
    imsak->setFont(textFont);
    fajr->setFont(textFont);
    sunrise->setFont(textFont);
    zuhr->setFont(textFont);
    asr->setFont(textFont);
    maghrib->setFont(textFont);
    sunset->setFont(textFont);
    isha->setFont(textFont);
    midnight->setFont(textFont);
    display->setFont(timeFont);
    DateToday->setFont(textFont);
    Layout->addWidget(display, 0, 1, 0);
    Layout->addWidget(DateToday, 1, 1, 0);
    Layout->addWidget(Country, 2, 0, 0);
    Layout->addWidget(City, 2, 1, 0);
    Layout->addWidget(Show, 2, 2, 0);
    Layout->addWidget(imsak, 3, 0, 0);
    Layout->addWidget(fajr, 3, 1, 0);
    Layout->addWidget(sunrise, 3, 2, 0);
    Layout->addWidget(zuhr, 4, 0, 0);
    Layout->addWidget(asr, 4, 1, 0);
    Layout->addWidget(maghrib, 4, 2, 0);
    Layout->addWidget(isha, 5, 0, 0);
    Layout->addWidget(sunset, 5, 1, 0);
    Layout->addWidget(midnight, 5, 2, 0);
    PrayerTimeWidget->setLayout(Layout);
    PrayerTimeWidget->setWindowTitle("Prayer Times");
    PrayerTimeWidget->setMinimumSize(500, 400);
    if(dark_mode_enabled)
        PrayerTimeWidget->setStyleSheet(ts.readAll());
    else
        PrayerTimeWidget->setStyleSheet("");
    PrayerTimeWidget->show();
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    connect(Show, SIGNAL(clicked()), this, SLOT(getPrayerTimes()));
    timer->start(1000);
}

void Window::showTime()
{
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh:mm AP");
    if((time.second() % 2) == 0)
    {
        time_text[2] = ' ';
    }
    display->setText(time_text);
}

void Window::getPrayerTimes()
{
    std::string apiurl = "http://api.aladhan.com/v1/timingsByCity?city=";
    std::string country = Country->text().toStdString();
    std::string city = City->text().toStdString();
    apiurl.append(city + "&country=" + country + "&method=4");
    for(int i=0;apiurl[i] != '\0';i++)
    {
        if(apiurl[i] == ' ')
            apiurl[i] = '%';
    }
    QString temp;
    this->url = apiurl;
    try{
        json result = json::parse(curl_process());
        temp = "Imsak: ";
        temp.append(QString::fromStdString(result["data"]["timings"]["Imsak"].get<std::string>()));
        imsak->setText(temp);
        temp = "Fajr: ";
        temp.append(QString::fromStdString(result["data"]["timings"]["Fajr"].get<std::string>()));
        fajr->setText(temp);
        temp = "Sunrise: ";
        temp.append(QString::fromStdString(result["data"]["timings"]["Sunrise"].get<std::string>()));
        sunrise->setText(temp);
        temp = "Zuhr: ";
        temp.append(QString::fromStdString(result["data"]["timings"]["Dhuhr"].get<std::string>()));
        zuhr->setText(temp);
        temp = "Asr: ";
        temp.append(QString::fromStdString(result["data"]["timings"]["Asr"].get<std::string>()));
        asr->setText(temp);
        temp = "Maghrib: ";
        temp.append(QString::fromStdString(result["data"]["timings"]["Maghrib"].get<std::string>()));
        maghrib->setText(temp);
        temp = "Isha: ";
        temp.append(QString::fromStdString(result["data"]["timings"]["Isha"].get<std::string>()));
        isha->setText(temp);
        temp = "Midnight: ";
        temp.append(QString::fromStdString(result["data"]["timings"]["Midnight"].get<std::string>()));
        midnight->setText(temp);
        temp = "Sunset: ";
        temp.append(QString::fromStdString(result["data"]["timings"]["Sunset"].get<std::string>()));
        sunset->setText(temp);
    } catch(nlohmann::json::type_error &err) {
        QMessageBox::critical(PrayerTimeWidget, "Error", "Invalid Country or City. Please Enter a valid location");
        
    }
}
