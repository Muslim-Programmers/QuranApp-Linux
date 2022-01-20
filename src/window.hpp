#pragma once
#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QCheckBox>
#include <vector>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QPixmap>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QGridLayout>
#include <QUrl>
#include <QtMultimedia/QtMultimedia>
#include <QProgressBar>
#include <QSlider>
#include <QStackedWidget>
#include "curl_parser.hpp"
#include "db_reader.hpp"

class Window : public QMainWindow , public CURLParser
{
    Q_OBJECT
    public:
        Window();
    private:
        int surah_number = 0;
        bool quran_is_playing = false;
        bool quran_is_paused = true;
        QUrl QuranUrl = QUrl("https://server8.mp3quran.net/afs/001.mp3");
        void createMenu();
        void createTaskbar();
        QGroupBox *createComboBox(); // GroupBox of Translation list and Surah List
        QGroupBox *createTextBox();  // GroupBox of Translation TextBox and Surah TextBox
        QGroupBox *createPlayerUi(); // GroupBox of Online Player
        QWidget *multiWidget();    // Widget with Quran Translation and Arabic Text
        QWidget *readingWidget();  // Widget with only Arabic Text
        QStackedWidget *stackedWidget = nullptr; 
        QPushButton *readingButton = nullptr; // Reading Button enables Arabic Text only view
        QPushButton *multiButton = nullptr;   // Multi Button enables both arabic text and translation
        QComboBox *surahList = nullptr;           // Surah List in Multimode
        QComboBox *readList  = nullptr;           // Surah List in Reading Mode
        QComboBox *translationList = nullptr;     // Available translation list
        QTextEdit *surahInMultimode = nullptr;        // Surah view Text Box in Multi Mode
        QTextEdit *translationInMulti = nullptr;  // Translation view Text Box in Multi Mode
        QTextEdit *surahInReadmode = nullptr;         // Surah view Text Box in Reading Mode
        QMenuBar *menuBar = nullptr;            // Menu Bar
        QMenu *Menu = nullptr;                  // Menu class
        QAction *about = nullptr;               // About menu Entry
        QAction *prayertimes = nullptr;         // Prayertimes menu Entry
        QTimer *timer = nullptr;                
        QLabel *display = nullptr;
        QLabel *imsak = nullptr;
        QLabel *fajr = nullptr;
        QLabel *sunrise = nullptr;
        QLabel *zuhr = nullptr;
        QLabel *asr = nullptr;
        QLabel *maghrib = nullptr;
        QLabel *isha = nullptr;
        QLabel *midnight = nullptr;
        QLabel *sunset = nullptr;
        QLineEdit *Country = nullptr;
        QLineEdit *City = nullptr;
        QPushButton *Show = nullptr;
        QWidget *PrayerTimeWidget = nullptr;    // Widget to show PrayerTimes
        QWidget *MediaPlayerWidget = nullptr;   // Online Player Widget
        QMediaPlayer Mediaplayer;
        QPushButton *play = nullptr;
        QPushButton *pause = nullptr;
        QPushButton *stop = nullptr;
        QSlider *positionSlider = nullptr;
        QLabel *positionLabel = nullptr;
        void getSurah(std::string, std::string, QTextEdit*);
        void getTranslation(std::string, std::string);
        QUrl getQuranUrl(int surah_number);
    private slots:
        void readingMode();
        void multiMode();
        void changeSurah();
        void changeTranslation();
        void changeSurahInRead();
        void showAbout();
        void showTime();
        void showPrayerTimes();
        void getPrayerTimes();
        void set_play();
        void set_pause();
        void set_stop();
        void updateDuration(qint64 duration);
        void updatePosition(qint64 position);
};

#endif
