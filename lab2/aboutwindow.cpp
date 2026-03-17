#include "aboutwindow.h"
#include "ui_aboutwindow.h"

aboutWindow::aboutWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::aboutWindow)
{
    ui->setupUi(this);
    ui->htmlBrowser->setHtml("<html><body>"
                             "<h2>Заголовок</h2>"
                             "<ul><li>Пункт 1</li><li>Пункт 2</li></ul>"
                             "<h2>Картинка</h2>"
                             "<img src=\"C:/CODE/OSNOV/lab2/1213.png\" />"
                             "<p> Hello World </p>"
                             "<img src=\"C:/CODE/OSNOV/lab2/1213.png\" />"
                             "</body></html>");
}

aboutWindow::~aboutWindow()
{
    delete ui;
}

void aboutWindow::on_backButton_clicked()
{
    emit backClicked();
    this->hide();
}

