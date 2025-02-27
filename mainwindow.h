#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>  // Để sử dụng QProcess
#include <QTextEdit> // Để sử dụng QTextEdit
#include <QTableView>
#include <QStandardItemModel>
#include <QGridLayout>
#include <QString>
#include <QTimer>

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
    void runExecutable(const QString &executable); // Slot để chạy file thực thi
    void readOutput();    // Slot để đọc đầu ra từ QProcess
    void readError();     // Slot để đọc lỗi từ QProcess
    void manageTask();
    void showDevInfo();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onSelectionChanged(const QItemSelection &selected);
    void updateTimer();
    void endTask();
    void destroyedPid();

private:
    Ui::MainWindow *ui;
    QProcess *process;    // Khai báo con trỏ QProcess
    QProcess *process_2;
    QProcess *process_3;
    QTextEdit *textEdit;  // QTextEdit để hiển thị kết quả
    QTableView *tableView;
    QStandardItemModel *model;
    QGridLayout *layout;
    QTimer  *timer;
    bool process_flag;
    int PID;
};

#endif // MAINWINDOW_H
