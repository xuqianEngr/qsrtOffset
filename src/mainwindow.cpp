#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"
#include "QTextStream"
#include "QDebug"
#include "QDir"
#include "QStringList"
#include "QDateTime"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_openFileButton_clicked()
{
    this->filePath = QFileDialog::getOpenFileName(this,"字幕文件选取","","*.srt ;; *.*");
    ui->filePathLineEdit->setText(this->filePath);
}

QString MainWindow::dealTimeString(QString string, int64_t ms)
{
    QStringList timeString;
    timeString = string.split(" --> ");
    if(timeString.count()>=2){
        timeString[0] = QDateTime::fromString(timeString[0], "hh:mm:ss,zzz").addMSecs(ms).toString("hh:mm:ss,zzz");
        timeString[1] = QDateTime::fromString(timeString[1], "hh:mm:ss,zzz").addMSecs(ms).toString("hh:mm:ss,zzz");
    }
    return timeString[0]+" -- >"+timeString[1];
}

void MainWindow::on_EnterButton_clicked()
{
    QStringList stringList;
    QFile file(this->filePath);
    int64_t ms = ui->offsetTimeLineEdit->text().toLongLong();
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
       QMessageBox::warning(this,tr("错误"),tr("打开文件失败"));
       return;
    }
    else{
       if(!file.isReadable()){
           QMessageBox::warning(this,tr("错误"),tr("该文件不可读"));
       }
       else{
            QTextStream textStream(&file);       //读取文件，使用QTextStream
            textStream.setCodec("GBK");
            uint32_t index = 1;
            while(!textStream.atEnd())
            {
                QString temp = textStream.readLine();
                if(temp == QString::number(index)){
                    stringList.append(temp);
                    stringList.append(this->dealTimeString(textStream.readLine(),ms));
                    index++;
                }
                else{
                    stringList.append(temp);
                }
            }
            file.close();
            this->writeFile(stringList);
       }
    }
}
bool MainWindow::writeFile(QStringList list)
{
    bool stage = false;
    QFile file(this->filePath+"add");
    file.open(QIODevice::WriteOnly);
    file.close();
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        stage = true;
        QTextStream stream(&file);
        stream.seek(file.size());
        foreach(QString s,list){
         stream << s << "\n";
        }
        file.close();
    }
    return stage;
}
