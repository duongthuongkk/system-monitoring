#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QGridLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QString>
#include <QHeaderView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Tạo các widget cần thiết
    QPushButton *dev_info = new QPushButton("Device Info", this);
    QPushButton *task_manager = new QPushButton("Processes", this);

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);  // Để không cho phép chỉnh sửa văn bản trong textEdit

    // Create a table of processes
    tableView = new QTableView(this);
    model = new QStandardItemModel(2, 3, this);

    // Tạo layout và thêm các widget vào
    layout = new QGridLayout();
    layout->addWidget(dev_info, 0, 0); // row, column
    layout->addWidget(task_manager, 1, 0); // row, column
    layout->addWidget(textEdit, 0, 1 ,10, 1);    // row, colum, number of row, number of column
    layout->addWidget(tableView, 0, 1, 10, 1);

    // Default display
    tableView->hide();

    // Adjust table

        // Đặt tiêu đề cột
    model->setHorizontalHeaderLabels(QStringList() << "Process name" << "pid" << "CPU" << "Memory");
        // Thêm dữ liệu
    model->setItem(0, 0, new QStandardItem("Nguyễn Văn A"));
    model->setItem(0, 1, new QStandardItem("30"));
    model->setItem(0, 2, new QStandardItem("Hà Nội"));

    model->setItem(1, 0, new QStandardItem("Trần Thị B"));
    model->setItem(1, 1, new QStandardItem("25"));
    model->setItem(1, 2, new QStandardItem("Hồ Chí Minh"));

        // Thiết lập mô hình cho table view
    tableView->setModel(model);

        // Change body style
    tableView->setStyleSheet(
        "QTableView {" 
            "font-family: Arial;"
            "font-size: 20px;"
            "text-align: center; "
            "border: 2px;"
            "padding: 5px"              // Adjust height of row
            "}"
        );

        // Hide index column
    tableView->verticalHeader()->setVisible(false);

        // Change title style
    tableView->horizontalHeader()->setStyleSheet(
        "QHeaderView::section { "
            "background-color: lightblue; "
            "color: darkblue; "
            "font-weight: bold; "
            "font-size: 22px; "
            "text-align: center; "
            "}");

    tableView->resizeRowsToContents();    // Điều chỉnh chiều cao hàng tự động theo nội dung
    tableView->resizeColumnsToContents(); // Điều chỉnh chiều rộng cột tự động theo nội dung
    tableView->horizontalHeader()->setStretchLastSection(true);


    // Adjust button size
    dev_info->setFixedSize(250,75);
    dev_info->setStyleSheet("QPushButton { font-size: 28px; color: blue }");
    task_manager->setFixedSize(250,75);
    task_manager->setStyleSheet("QPushButton { font-size: 28px; color: blue }");

    // Adjust font size and line spacing
    textEdit->setStyleSheet("QTextEdit {"
                            "font-family: Arial;"
                            "font-size: 25px; "
                            "line-height: 15px;" "}");
    
    textEdit->setMinimumSize(640, 480);  // Kích thước tối thiểu là 200x100
    textEdit->setMaximumSize(1200,800);  // Kích thước tối đa là 600x400
                                                    

    // Tạo một QWidget trung gian và đặt layout cho nó
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Kết nối tín hiệu nút bấm với slot chạy file thực thi
    connect(dev_info, &QPushButton::clicked, this, &MainWindow::showDevInfo);
    connect(task_manager, &QPushButton::clicked, this, &MainWindow::manageTask);

    process = new QProcess(this);
    // Kết nối các tín hiệu của QProcess để đọc kết quả hoặc lỗi
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::readOutput);
    connect(process, &QProcess::readyReadStandardError, this, &MainWindow::readError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::runExecutable(const QString &executable)
{   
    // Khởi động quá trình thực thi
    process->start(executable);
// Kiểm tra nếu quá trình đã bắt đầu thành công
   if (!process->waitForFinished()) {
       qDebug() << "Failed to start process.";
   }
}

void MainWindow::showDevInfo(){
    textEdit->clear();
    tableView->hide();
    textEdit->show();
    
    runExecutable("/home/bbb/Desktop/Qt/hello/system/device_info");
}

void MainWindow::manageTask()
{
    // Set all columns to stretchable so they adjust when resizing the window
    for (int i = 0; i < model->columnCount(); ++i) {
        tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }

    //Change display
    textEdit->hide();
    tableView->show();
    
}

void MainWindow::readOutput()
{
    // Đọc output từ file thực thi và hiển thị trong QTextEdit
    QString output = process->readAllStandardOutput();
    textEdit->append(output);
}

void MainWindow::readError()
{
    // Đọc lỗi từ file thực thi và hiển thị trong QTextEdit
    QString error = process->readAllStandardError();
    textEdit->append("Error: " + error);
}
