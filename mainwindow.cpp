#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
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
#include <QTimer>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Turn off process update
    process_flag = false;

    // Tạo các widget cần thiết
    QPushButton *dev_info = new QPushButton("Device Info", this);
    QPushButton *task_manager = new QPushButton("Processes", this);
    QPushButton *end_task = new QPushButton("End task", this);

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
    layout->addWidget(end_task, 10, 1, Qt::AlignRight);

    // Default display
    tableView->hide();

    // Adjust table

        // Đặt tiêu đề cột
    model->setHorizontalHeaderLabels(QStringList() << "Process name" << "Pid" << "User" << "CPU(%)" << "Memory(%)");

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
    end_task->setFixedSize(120,40);
    end_task->setStyleSheet("QPushButton { font-size: 20px; text-align: center}");
    end_task->setIcon(QIcon("/home/bbb/Desktop/Qt/hello/assets/ban-svgrepo-com.svg"));

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
    connect(tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &MainWindow::onSelectionChanged);
    connect(end_task,&QPushButton::clicked, this, &MainWindow::destroyedPid);

    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  // Chọn cả hàng
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);  // Chọn một hàng tại một thời điểm
                

    // Tạo đối tượng QProcess cho DEVICE INFO
    process = new QProcess(this);

    // Tạo đối tượng QProcess cho PROCESSES
    process_2 = new QProcess(this);

    // Tạo đối tượng QProcess cho PROCESSES
    process_3 = new QProcess(this);

    // Kết nối các tín hiệu của QProcess để đọc kết quả hoặc lỗi
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::readOutput);
    connect(process, &QProcess::readyReadStandardError, this, &MainWindow::readError);
    connect(process_2, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));
    connect(process_3,&QProcess::started, this, &MainWindow::endTask);
    // Chạy lệnh `ps aux` để lấy thông tin process
    process_2->start("ps", QStringList() << "aux");



    // QTimer setup
        // Tạo QTimer để cập nhật mỗi giây
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
        timer->start(500);  // update after time
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
    process_flag =false;
}

void MainWindow::manageTask()
{
    process_flag = true;
    // Set all columns to stretchable so they adjust when resizing the window
    for (int i = 0; i < model->columnCount(); ++i) {
        tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }

    //Change display
    textEdit->hide();
    tableView->show();
}

void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus){
    // Processing pid
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        // Lấy đầu ra từ QProcess
        QString output = process_2->readAllStandardOutput();

        // Tách các dòng thông qua \n
        QStringList lines = output.split('\n', QString::SkipEmptyParts);

        // Chỉ xóa model nếu có dữ liệu cũ
        if (model->rowCount() > 0) {
            model->removeRows(0, model->rowCount());
        }

        // Xóa dòng đầu tiên (tiêu đề cột)
        if (!lines.isEmpty()) {
            lines.removeFirst();
        }

        // Duyệt qua từng dòng và lấy PID (cột thứ 2 trong output của ps aux)
        for (const QString &line : lines) {
            QStringList columns = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            if (columns.size() > 2) { // The line starting to get data
                
                // Tạo attribute
                    // Loại bỏ ký tự '[' và ']'
                QString pnameStr = columns[10];
                pnameStr.replace("[", "");  // Thay thế '[' bằng chuỗi rỗng
                pnameStr.replace("]", "");  // Thay thế ']' bằng chuỗi rỗng

                QStandardItem *user = new QStandardItem(columns[0]);
                QStandardItem *pid = new QStandardItem(columns[1]);
                QStandardItem *cpu = new QStandardItem(columns[2]);
                QStandardItem *memory = new QStandardItem(columns[3]);
                QStandardItem *pname = new QStandardItem(pnameStr);

                //Set read only
                user->setFlags(user->flags() & ~Qt::ItemIsEditable);
                pid->setFlags(pid->flags() & ~Qt::ItemIsEditable);
                cpu->setFlags(cpu->flags() & ~Qt::ItemIsEditable);
                memory->setFlags(memory->flags() & ~Qt::ItemIsEditable);
                pname->setFlags(pname->flags() & ~Qt::ItemIsEditable);

                // Thêm attribute vào model
                model->appendRow({pname, pid, user, cpu, memory});
            }
        }
    }
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
void MainWindow::onSelectionChanged(const QItemSelection &selected) {
    QModelIndexList selectedIndexes = selected.indexes();  // Lấy danh sách chỉ số của các ô được chọn

    // Kiểm tra xem có bất kỳ ô nào được chọn không
    if (!selectedIndexes.isEmpty()) {
        // Lấy chỉ số của hàng đầu tiên trong selection
        int row = selectedIndexes.first().row();

        // Chọn toàn bộ hàng của ô được chọn
        tableView->selectRow(row);
    } else {
        // Nếu không có ô nào được chọn, hủy chọn hàng
        tableView->clearSelection();
    }
}

void MainWindow::updateTimer(){
    if (process_2->state() == QProcess::NotRunning && process_flag) {  
        qDebug() << "Starting new process...";
        process_2->start("ps", QStringList() << "aux");
    } else {
        qDebug() << "Previous process still running!";
    }
}

void MainWindow::endTask(){
    QStringList args;
    args << QString::number(PID);  // Chuyển PID thành chuỗi và đưa vào danh sách đối số
    process_3->start("/home/bbb/Desktop/Qt/hello/system/manage_process",args);
}

void MainWindow::destroyedPid(){
    PID = 10000;
    qDebug()<<"Successfully send PID to end task";
}