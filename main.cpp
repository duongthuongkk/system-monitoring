#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle("System Monitoring");
    // Tạo đối tượng QProcess để chạy file thực thi
    return a.exec();
}
