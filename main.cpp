


#include <QApplication>
#include <QSettings>
#include <QDesktopWidget>
#include <QDebug>
#include <cwindows.h>
#include <QPushButton>
#include <QTabWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CWindows* windows =  new  CWindows();
    windows->InitWidget();
    a.quit();
    return a.exec();
}

