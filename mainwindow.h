#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>  // Để sử dụng QProcess
#include <QTextEdit> // Để sử dụng QTextEdit

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void runExecutable(); // Slot để chạy file thực thi
    void readOutput();    // Slot để đọc đầu ra từ QProcess
    void readError();     // Slot để đọc lỗi từ QProcess

private:
    Ui::MainWindow *ui;
    QProcess *process;    // Khai báo con trỏ QProcess
    QTextEdit *textEdit;  // QTextEdit để hiển thị kết quả
};

#endif // MAINWINDOW_H
