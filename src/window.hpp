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
#include "curl_parser.hpp"
#include "db_reader.hpp"

class Window : public QMainWindow , public CURLParser
{
    Q_OBJECT
    public:
        Window();
    private:
        int surah_number = 0;
        bool dark_mode_enabled = false;
        QGroupBox *createComboBox();
        QGroupBox *createTextBox();
        void createMenu();
        QComboBox *surah;
        QComboBox *translation;
        QTextEdit *show_surah;
        QTextEdit *show_translation;
        QMenuBar *menuBar;
        QMenu *Menu;
        QAction *about;
        QAction *darkmode;
        QAction *prayertimes;
        QTimer *timer;
        QLabel *display;
        QLabel *imsak;
        QLabel *fajr;
        QLabel *sunrise;
        QLabel *zuhr;
        QLabel *asr;
        QLabel *maghrib;
        QLabel *isha;
        QLabel *midnight;
        QLabel *sunset;
        QLineEdit *Country;
        QLineEdit *City;
        QPushButton *Show;
        QWidget *PrayerTimeWidget;
        void getSurah(std::string, std::string);
        void getTranslation(std::string, std::string);
    private slots:
        void showSurah();
        void showTranslation();
        void showAbout();
        void setDarkMode();
        void showTime();
        void showPrayerTimes();
        void getPrayerTimes();
};

#endif