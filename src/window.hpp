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
#include "parser.hpp"

class Window : public QMainWindow , public Parser
{
    Q_OBJECT
    public:
        Window();
    private:
        QGroupBox *createComboBox();
        QGroupBox *createTextBox();
        std::vector<std::string> getMetadata();
};

#endif