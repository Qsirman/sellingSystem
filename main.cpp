#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QFile goodsInfo("C:\\Users\\leanard\\Documents\\sellingSystem\\goodsInfo.xml");
    QApplication a(argc, argv);
    MainWindow w(&goodsInfo);
    w.show();
    return a.exec();
}
