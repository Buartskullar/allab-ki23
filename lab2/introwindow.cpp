#include "introwindow.h"
#include "ui_introwindow.h"

introWindow::introWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::introWindow)
{
    ui->setupUi(this);

    workWin = new MainWindow;
    aboutWin = new aboutWindow;
    connect(workWin,&MainWindow::backClicked, this, &QMainWindow::show);
    connect(aboutWin,&aboutWindow::backClicked, this, &QMainWindow::show);
    connect(ui->buttonExit, &QPushButton::clicked, this, &QApplication::quit);
}

introWindow::~introWindow()
{
    delete ui;
}

void introWindow::on_buttonWork_clicked()
{
    this->hide();
    workWin->show();
}


void introWindow::on_buttonAbout_clicked()
{
    this->hide();
    aboutWin->show();
}

