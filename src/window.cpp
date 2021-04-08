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
    MainLayout->addWidget(createPlayerUi());   // set PlayerUi
    MainLayout->addWidget(createTextBox());    // set TextBox
    MainWidget->setLayout(MainLayout);         // set Layout
    setCentralWidget(MainWidget);              // set Main Widget
    setWindowTitle("Quran App");               // set Window Title
    setMinimumWidth(800);                      // set Minimum Width of Window
    setMinimumHeight(600);                     // set Minimum Height of Window
    connect(surah, SIGNAL(currentTextChanged(QString)), this, SLOT(showSurah())); // Watch Changes in ComboBox surah
    connect(translation, SIGNAL(currentTextChanged(QString)), this, SLOT(showTranslation())); // Watch Changes in ComboBox translation
    connect(play, SIGNAL(clicked()), this, SLOT(set_play()));
    connect(pause, SIGNAL(clicked()), this, SLOT(set_pause()));
    connect(stop, SIGNAL(clicked()), this, SLOT(set_stop()));
}

void Window::createMenu()
{
    // Function to Configure MenuBar

    menuBar = new QMenuBar;
    Menu = new QMenu(tr("Menu"), this);
    darkmode = Menu->addAction(tr("Dark Mode")); // Add Dark Mode Menu Entry
    darkmode->setCheckable(true);
    prayertimes = Menu->addAction(tr("Prayer Times"));
    about = Menu->addAction(tr("About"));        // Add About Menu Entry
    menuBar->addMenu(Menu);
    connect(about, SIGNAL(triggered()), this, SLOT(showAbout())); 
    connect(darkmode, SIGNAL(triggered()), this, SLOT(setDarkMode()));
    connect(prayertimes, SIGNAL(triggered()), this, SLOT(showPrayerTimes()));
}

QGroupBox *Window::createComboBox()
{
    // Function to Configure ComboBox
    QDBReader Database;
    std::vector<std::string>data = Database.metadata();
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

    translation->setCurrentIndex(5);
    QGroupBox *group = new QGroupBox;
    QHBoxLayout *layout = new QHBoxLayout;
    show_surah = new QTextEdit;
    show_translation = new QTextEdit;
    getSurah("Al_Faatiha", "quran");
    getTranslation("Al_Faatiha", "English");
    layout->addWidget(show_translation);
    layout->addWidget(show_surah);
    group->setLayout(layout);
    return group;
}

QGroupBox *Window::createPlayerUi()
{
    // create Media Player UI

    QGroupBox *PlayerBox = new QGroupBox(tr("Play Surah"));
    QHBoxLayout *PlayerLayout = new QHBoxLayout;
    play = new QPushButton("Play");
    pause = new QPushButton("Pause");
    stop = new QPushButton("Stop");
    player = new QMediaPlayer;
    player->setMedia(QuranUrl);
    PlayerLayout->addWidget(play);
    PlayerLayout->addWidget(pause);
    PlayerLayout->addWidget(stop);
    PlayerBox->setLayout(PlayerLayout);
    return PlayerBox;

}


QUrl Window::getQuranUrl(int surah_number)
{
    QUrl qurl;
    std::string QuranUrl = "https://server8.mp3quran.net/afs/";
    if(surah_number >= 1 && surah_number <= 9)
    {
        QuranUrl.append("00");
        QuranUrl.append(std::to_string(surah_number));
        QuranUrl.append(".mp3");
    }
    else if(surah_number >= 10 && surah_number <= 99)
    {
        QuranUrl.append("0");
        QuranUrl.append(std::to_string(surah_number));
        QuranUrl.append(".mp3");
    }
    else if(surah_number >= 100 && surah_number <= 114)
    {
        QuranUrl.append(std::to_string(surah_number));
        QuranUrl.append(".mp3");
    }

    qurl = QuranUrl.c_str();
    return qurl;
}

void Window::getSurah(std::string surah_name, std::string edition)
{
    QDBReader Database;
    std::vector<std::string> data;
    std::vector<std::string> meta = Database.metadata();
    show_surah->setText(QString::fromStdString(meta.at(surah_number)) + "\n");
    QTextCursor cursor = show_surah->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignCenter);              // Align Text to Center
    cursor.mergeBlockFormat(textBlockFormat);
    data = Database.content(Database.rep((char*)surah_name.c_str()), Database.getedition(edition));
    for(auto str : data)
        show_surah->append(QString::fromStdString(str) + "\n");
    show_surah->selectAll();
    show_surah->setFontPointSize(16); // set Font Size
    show_surah->setTextCursor(cursor);
    show_surah->setReadOnly(true);    // set Text Box Read Only
}

void Window::getTranslation(std::string translation_name, std::string edition)
{
    QDBReader Database;
    std::vector<std::string> data;
    std::vector<std::string> meta = Database.metadata();
    show_translation->setText(QString::fromStdString(meta.at(surah_number)) + "\n");
    QTextCursor cursor = show_translation->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignCenter);
    cursor.mergeBlockFormat(textBlockFormat);
    data = Database.content(Database.rep((char*)translation_name.c_str()), Database.getedition(edition));
    for(auto str : data)
        show_translation->append(QString::fromStdString(str) + "\n");
    show_translation->selectAll();
    show_translation->setFontPointSize(16);
    show_translation->setTextCursor(cursor);
    show_translation->setReadOnly(true);
}


void Window::showSurah()
{
    QDBReader Database;
    std::vector<std::string>data = Database.metadata();
    surah_number = surah->currentIndex();
    getSurah(data.at(surah_number), "quran");
    getTranslation(data.at(surah_number), translation->currentText().toStdString());
    QuranUrl = getQuranUrl(surah->currentIndex()+1);
    player->stop();
    player->setMedia(QuranUrl);
}

void Window::showTranslation()
{
    QDBReader Database;
    std::vector<std::string>data = Database.metadata();
    surah_number = surah->currentIndex();
    getTranslation(data.at(surah_number), translation->currentText().toStdString());
}

void Window::showAbout()
{
    QWidget *AboutWindow = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    QPixmap pixmap("resources/qapp-198x198.png");
    QFile f(":qdarkstyle/style.qss");
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
    QLabel *Contributers = new QLabel("Contributers : Nashid , Jonas");
    QLabel *footer = new QLabel("https://www.muslimprogrammers.com");
    QLabel *version = new QLabel("Version 1.4 beta");
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
    version->setFont(nfont);
    version->setAlignment(Qt::AlignCenter);
    layout->addWidget(icon);
    layout->addWidget(header);
    layout->addWidget(Discord);
    layout->addWidget(Instagram);
    layout->addWidget(Contributers);
    layout->addWidget(footer);
    layout->addWidget(version);
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
    QFile f(":qdarkstyle/style.qss"); 
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
    PrayerTimeWidget = new QWidget;
    QGridLayout *Layout = new QGridLayout; 
    QFile f(":qdarkstyle/style.qss");
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
    PrayerTimeWidget->setMinimumSize(600, 400);
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
        QMessageBox::critical(PrayerTimeWidget, "Error", "Make sure you have connected to internet and entered a valid location , or maybe the API is down");
        std::cerr << err.what() << std::endl;
    }
}

void Window::set_play()
{
    if(!quran_is_playing || quran_is_paused)
    {
        player->play();
        quran_is_playing = true;
    }
}

void Window::set_pause()
{
    if(quran_is_playing)
    {
        player->pause();
        quran_is_paused = true;
        quran_is_playing = false;
    }
}

void Window::set_stop()
{
    if(quran_is_playing || quran_is_paused)
    {
        player->stop();
        quran_is_playing = false;
        quran_is_paused = false;
    }
}