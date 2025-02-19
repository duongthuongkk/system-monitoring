#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Tạo các widget cần thiết
    QPushButton *runButton = new QPushButton("Run Executable", this);
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);  // Để không cho phép chỉnh sửa văn bản trong textEdit

    // Tạo layout và thêm các widget vào
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(runButton);
    layout->addWidget(textEdit);

    // Tạo một QWidget trung gian và đặt layout cho nó
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Kết nối tín hiệu nút bấm với slot chạy file thực thi
    connect(runButton, &QPushButton::clicked, this, &MainWindow::runExecutable);

    // Tạo đối tượng QProcess để chạy file thực thi
    process = new QProcess(this);

    // Kết nối các tín hiệu của QProcess để đọc kết quả hoặc lỗi
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::readOutput);
    connect(process, &QProcess::readyReadStandardError, this, &MainWindow::readError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::runExecutable()
{
    // Đường dẫn tới file thực thi đã biên dịch từ mã C của bạn
    QString executable = "/home/bbb/Desktop/Qt/hello/system/cpu";  // Thay bằng đường dẫn chính xác đến file thực thi của bạn

    // Khởi động quá trình thực thi
    process->start(executable);

    // Kiểm tra nếu quá trình không thể bắt đầu
    if (!process->waitForStarted()) {
        textEdit->append("Failed to start process.");
    }
}

void MainWindow::readOutput()
{
    // Đọc output từ file thực thi và hiển thị trong QTextEdit
    QString output = process->readAllStandardOutput();
    textEdit->append("Output: " + output);
}

void MainWindow::readError()
{
    // Đọc lỗi từ file thực thi và hiển thị trong QTextEdit
    QString error = process->readAllStandardError();
    textEdit->append("Error: " + error);
}
