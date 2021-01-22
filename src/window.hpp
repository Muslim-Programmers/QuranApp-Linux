#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <QMainWindow>
#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTextEdit>
#include <vector>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include "parser.hpp"

class Window : public QMainWindow , public Parser
{
    Q_OBJECT
    public:
        Window();
    private:
        int surah_number = 1;
        std::string edition = "en.sahih";
        QGroupBox *createComboBox();
        QGroupBox *createTextBox();
        void createMenu();
        std::vector<std::string> getMetadata();
        QComboBox *surah;
        QComboBox *translation;
        QTextEdit *show_surah;
        QTextEdit *show_translation;
        QMenuBar *menuBar;
        QMenu *Menu;
        QAction *about;
        void getSurah(std::string surah_url);
        void getTranslation(std::string translation_url);
        std::string getEdition(std::string identifier);
    private slots:
        void showSurah();
        void showTranslation();
        void showAbout();
};

#endif