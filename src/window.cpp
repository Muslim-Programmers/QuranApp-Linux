#include <iostream>
#include <string>
#include <QFile>
#include <QTextStream>
#include <QFontDatabase>
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
    QWidget *mainWidget = new QWidget; // Main Widget
    QVBoxLayout *mainLayout = new QVBoxLayout;   // Main Layout
    QGroupBox *tabularButtonGroup = new QGroupBox;  // button group to switch between reading mode and multi mode
    QHBoxLayout *tabularLayout = new QHBoxLayout;
    stackedWidget = new QStackedWidget;
    multiButton = new QPushButton("Translation");
    readingButton = new QPushButton("Reading");
    multiButton->setToolTip("Translation and Arabic View");
    readingButton->setToolTip("Only Arabic");
    tabularLayout->addWidget(multiButton);
    tabularLayout->addWidget(readingButton);
    tabularButtonGroup->setLayout(tabularLayout);
    createMenu();                              // Call createMenu() to configure MenuBar
    setMenuBar(menuBar);                       // set MenuBar
    stackedWidget->addWidget(multiWidget());
    stackedWidget->addWidget(readingWidget());
    stackedWidget->setCurrentIndex(0);
    mainLayout->addWidget(stackedWidget);
    mainLayout->addWidget(tabularButtonGroup);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);              // set Main Widget
    setWindowTitle("Quran App");               // set Window Title
    setMinimumWidth(800);                      // set Minimum Width of Window
    setMinimumHeight(600);                     // set Minimum Height of Window
    connect(multiButton, SIGNAL(clicked()), this, SLOT(multiMode()));
    connect(readingButton, SIGNAL(clicked()), this, SLOT(readingMode()));
    connect(surahList, SIGNAL(currentTextChanged(QString)), this, SLOT(changeSurah())); // Watch Changes in ComboBox surah
    connect(readList, SIGNAL(currentTextChanged(QString)), this, SLOT(changeSurahInRead()));
    connect(translationList, SIGNAL(currentTextChanged(QString)), this, SLOT(changeTranslation())); // Watch Changes in ComboBox translation
    connect(play, SIGNAL(clicked()), this, SLOT(set_play()));
    connect(pause, SIGNAL(clicked()), this, SLOT(set_pause()));
    connect(stop, SIGNAL(clicked()), this, SLOT(set_stop()));
    connect(&Mediaplayer, &QMediaPlayer::durationChanged, this, &Window::updateDuration);
    connect(&Mediaplayer, &QMediaPlayer::positionChanged, this, &Window::updatePosition);
}

void Window::createMenu()
{
    // Function to Configure MenuBar

    menuBar = new QMenuBar;
    Menu = new QMenu(tr("Menu"), this);
    prayertimes = Menu->addAction(tr("Prayer Times"));
    about = Menu->addAction(tr("About"));        // Add About Menu Entry
    menuBar->addMenu(Menu);
    connect(about, SIGNAL(triggered()), this, SLOT(showAbout())); 
    connect(prayertimes, SIGNAL(triggered()), this, SLOT(showPrayerTimes()));
}

QWidget *Window::multiWidget() {
    QWidget *MultiWidget = new QWidget;         
    QVBoxLayout *MultiLayout = new QVBoxLayout; 
    MultiLayout->addWidget(createComboBox());   
    MultiLayout->addWidget(createPlayerUi());   
    MultiLayout->addWidget(createTextBox());    
    MultiWidget->setLayout(MultiLayout);
    return MultiWidget;
}

QWidget *Window::readingWidget() {
    QWidget *ReadingWidget = new QWidget;
    QVBoxLayout *ReadingLayout = new QVBoxLayout;
    surahInReadmode= new QTextEdit;
    getSurah("Al_Faatiha", "quran", surahInReadmode);
    ReadingLayout->addWidget(readList);
    ReadingLayout->addWidget(surahInReadmode);
    ReadingWidget->setLayout(ReadingLayout);
    return ReadingWidget;
}

QGroupBox *Window::createComboBox()
{
    // Function to Configure ComboBox
    QDBReader Database;
    std::vector<std::string>data = Database.metadata();
    QGroupBox *group = new QGroupBox;      // GroupBox ( To Group Widgets )
    QHBoxLayout *layout = new QHBoxLayout; // Horizontal Layout
    surahList = new QComboBox;      
    readList = new QComboBox;
    translationList = new QComboBox;
    if(data.empty())
    {
        surahList->addItem("Database is Missing"); 
        readList->addItem("Database is Missing");
    }
    else
    {
        for(int i=0;i<114;i++)
        {
            surahList->addItem(QString::number(i+1) + ". " + QString::fromStdString(data.at(i)));
            readList->addItem(QString::number(i+1) + ". " + QString::fromStdString(data.at(i)));
        }
    }
    for(auto element : languages)
    {
        translationList->addItem(QString::fromStdString(element));
    }
    layout->addWidget(translationList);
    layout->addWidget(surahList);
    group->setLayout(layout);
    return group;
}

QGroupBox *Window::createTextBox()
{
    // Function to configure Text Box

    translationList->setCurrentIndex(5);
    QGroupBox *group = new QGroupBox;
    QHBoxLayout *layout = new QHBoxLayout;
    surahInMultimode = new QTextEdit;
    translationInMulti = new QTextEdit;
    getSurah("Al_Faatiha", "quran", surahInMultimode);
    getTranslation("Al_Faatiha", "English");
    layout->addWidget(translationInMulti);
    layout->addWidget(surahInMultimode);
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
    positionSlider = new QSlider(Qt::Horizontal);
    positionLabel = new QLabel(tr("00:00"));
    positionLabel->setMinimumWidth(positionLabel->sizeHint().width());
    positionSlider->setEnabled(false);
    Mediaplayer.setMedia(QuranUrl);
    PlayerLayout->addWidget(play);
    PlayerLayout->addWidget(pause);
    PlayerLayout->addWidget(stop);
    PlayerLayout->addWidget(positionSlider);
    PlayerLayout->addWidget(positionLabel);
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

void Window::getSurah(std::string surah_name, std::string edition, QTextEdit* textbox)
{
    QDBReader Database;
    std::vector<std::string> data;
    std::vector<std::string> meta = Database.metadata();
    int id = QFontDatabase::addApplicationFont("/opt/qapp/resources/Harmattan-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont arabic(family);
    textbox->setText(QString::fromStdString(meta.at(surah_number)));
    QTextCursor cursor = textbox->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    textBlockFormat.setAlignment(Qt::AlignCenter);              // Align Text to Center
    cursor.mergeBlockFormat(textBlockFormat);
    data = Database.content(Database.replaceStr((char*)surah_name.c_str()), Database.getedition(edition));
    for(auto str : data)
        textbox->append(QString::fromStdString(str));
    textbox->selectAll();
    textbox->setFont(arabic);
    textbox->setFontPointSize(22); // set Font Size
    textbox->setTextCursor(cursor);
    textbox->setReadOnly(true);    // set Text Box Read Only
}

void Window::getTranslation(std::string translation_name, std::string edition)
{
    QDBReader Database;
    std::vector<std::string> data;
    std::vector<std::string> meta = Database.metadata();
    translationInMulti->setText(QString::fromStdString(meta.at(surah_number)) + "\n");
    QTextCursor cursor = translationInMulti->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();
    cursor.mergeBlockFormat(textBlockFormat);
    data = Database.content(Database.replaceStr((char*)translation_name.c_str()), Database.getedition(edition));
    for(auto str : data)
        translationInMulti->append(QString::fromStdString(str) + "\n");
    translationInMulti->selectAll();
    translationInMulti->setFontPointSize(16);
    translationInMulti->setTextCursor(cursor);
    translationInMulti->setReadOnly(true);
}

void Window::multiMode() {
    stackedWidget->setCurrentIndex(0);
}

void Window::readingMode() {
    stackedWidget->setCurrentIndex(1);
}

void Window::changeSurah()
{
    QDBReader Database;
    std::vector<std::string>data = Database.metadata();
    surah_number = surahList->currentIndex();
    getSurah(data.at(surah_number), "quran", surahInMultimode);
    getTranslation(data.at(surah_number), translationList->currentText().toStdString());
    QuranUrl = getQuranUrl(surahList->currentIndex()+1);
    Mediaplayer.stop();
    Mediaplayer.setMedia(QuranUrl);
}

void Window::changeSurahInRead() {
    QDBReader Database;
    std::vector<std::string>data = Database.metadata();
    surah_number = readList->currentIndex();
    getSurah(data.at(surah_number), "quran", surahInReadmode);
}

void Window::changeTranslation()
{
    QDBReader Database;
    std::vector<std::string>data = Database.metadata();
    surah_number = surahList->currentIndex();
    getTranslation(data.at(surah_number), translationList->currentText().toStdString());
}

void Window::showAbout()
{
    QWidget *AboutWindow = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    QPixmap pixmap("/opt/qapp/resources/qapp-198x198.png");
    QFont hfont("Arial", 15, QFont::Bold);
    QFont nfont("Arial", 13, QFont::AnyStyle);
    QFont ffont("Arial", 12, QFont::AnyStyle);
    ffont.setItalic(true);
    QLabel *icon = new QLabel;
    QLabel *header = new QLabel("A Project by Muslim Programmers Community");
    QLabel *Instagram = new QLabel("Instagram : @muslimpgmrs");
    QLabel *Contributers = new QLabel("Contributers : Nashid , Jonas");
    QLabel *footer = new QLabel("https://www.muslimprogrammers.com");
    QLabel *version = new QLabel("Version 1.5 stable");
    icon->setPixmap(pixmap);
    icon->setAlignment(Qt::AlignCenter);
    icon->setGeometry(QRect(312, 454, 21, 20));
    header->setAlignment(Qt::AlignCenter);
    header->setFont(hfont);
    header->setGeometry(QRect(312, 454, 21, 20));
    Instagram->setFont(nfont);
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
    layout->addWidget(Instagram);
    layout->addWidget(Contributers);
    layout->addWidget(footer);
    layout->addWidget(version);
    AboutWindow->setLayout(layout);
    AboutWindow->setMinimumSize(602,443);
    AboutWindow->setWindowTitle("About");
    AboutWindow->show();
}

void Window::showPrayerTimes()
{
    PrayerTimeWidget = new QWidget;
    QGridLayout *Layout = new QGridLayout; 
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
        Mediaplayer.play();
        quran_is_playing = true;
        positionSlider->setEnabled(true);
    }
}

void Window::set_pause()
{
    if(quran_is_playing)
    {
        Mediaplayer.pause();
        quran_is_paused = true;
        quran_is_playing = false;
    }
}

void Window::set_stop()
{
    if(quran_is_playing || quran_is_paused)
    {
        Mediaplayer.stop();
        quran_is_playing = false;
        quran_is_paused = false;
        positionSlider->setEnabled(false);
    }
}

static QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

void Window::updateDuration(qint64 duration)
{
    positionSlider->setRange(0, duration);
    positionSlider->setEnabled(duration > 0);
    positionSlider->setPageStep(duration / 10);
}

void Window::updatePosition(qint64 position)
{
    positionSlider->setValue(position);
    positionLabel->setText(formatTime(position));
}
