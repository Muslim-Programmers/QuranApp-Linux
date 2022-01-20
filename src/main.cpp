#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include "window.hpp"

int main(int argc, char *argv[])
{
    // Initiate Qt Application
    QApplication qapp(argc, argv);
    qapp.setWindowIcon(QIcon("/opt/qapp/resources/icon.png"));
    Window *window;                                         // Main Window
    QPixmap pixmap("/opt/qapp/resources/qapp-splash.jpg");            // Image to show in Splash Screen
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint); // Splash Screen object    
    window = new Window;
    splash.show(); // Show Splash Screen
    QTimer::singleShot(3000, &splash, &QWidget::close); // Keep Splash Screen for 3 seconds
    window->show(); // Show Main Window
    return qapp.exec();
}
