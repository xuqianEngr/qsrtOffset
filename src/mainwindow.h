#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openFileButton_clicked();
    void on_EnterButton_clicked();

private:
    Ui::MainWindow *ui;
    QString filePath = NULL;
    QString dealTimeString(QString string, int64_t ms);
    bool writeFile(QStringList list);
};
#endif // MAINWINDOW_H
