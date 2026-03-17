#ifndef INTROWINDOW_H
#define INTROWINDOW_H

#include <QWidget>
#include "mainwindow.h"
#include "aboutwindow.h"

namespace Ui {
class introWindow;
}

class introWindow : public QWidget
{
    Q_OBJECT

public:
    explicit introWindow(QWidget *parent = nullptr);
    ~introWindow();

private slots:
    void on_buttonAbout_clicked();
    void on_buttonWork_clicked();

    void on_buttonExit_clicked();

private:
    Ui::introWindow *ui;
    MainWindow *workWin;
    aboutWindow *aboutWin;
};

#endif // INTROWINDOW_H
