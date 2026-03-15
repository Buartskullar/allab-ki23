#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    income_l = new incomeList(this);
    ui->mainList->setModel(income_l);
    income_l->loadFromFile();

    connect(ui->mainList, &QListView::clicked, this, &::MainWindow::onListClicked);

    ui->lineSum->setValidator(new QIntValidator(-100000000, 100000000, this));
    ui->lineDay->setInputMask("99.99.9999;_");
    ui->linePlace->setValidator(new QRegularExpressionValidator(QRegularExpression("[^\":]*"), this));

    connect(ui->lineName, &QLineEdit::textChanged, this, &::MainWindow::updateName);
    connect(ui->linePlace, &QLineEdit::textChanged, this, &::MainWindow::updatePlace);
    connect(ui->lineDay, &QLineEdit::textChanged, this, &::MainWindow::updateDay);
    connect(ui->lineSum, &QLineEdit::textChanged, this, &::MainWindow::updateSum);

    auto saveLayout = [this]() {
        income_l->saveToFile();
    };

    connect(income_l, &incomeList::dataChanged, this, saveLayout);
    connect(income_l, &incomeList::rowsInserted, this, saveLayout);
    connect(income_l, &incomeList::rowsRemoved, this, saveLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateName(const QString &name){
    if (!currentIndex.isValid()) return;

    ui->mainList->model()->setData(currentIndex, name, incomeList::NameRole);
}
void MainWindow::updateDay(const QString &day){
    if (!currentIndex.isValid()) return;

    ui->mainList->model()->setData(currentIndex, day, incomeList::DayRole);
}
void MainWindow::updatePlace(const QString &place){
    if (!currentIndex.isValid()) return;

    ui->mainList->model()->setData(currentIndex, place, incomeList::PlaceRole);
}
void MainWindow::updateSum(const QString &sum){
    if (!currentIndex.isValid()) return;

    ui->mainList->model()->setData(currentIndex, sum, incomeList::SumRole);
}

void MainWindow::onListClicked(const QModelIndex &index) {
    if (!index.isValid()) return;

    const income &obj = income_l->getItem(index);
    currentIndex = index;

    ui->lineName->setText(obj.getName());
    ui->lineSum->setText(QString::number(obj.getSum()));
    ui->lineDay->setText(obj.getDay());
    ui->linePlace->setText(obj.getPlace());
}


void MainWindow::on_buttonAdd_clicked()
{
    income_l->addObject();
}
void MainWindow::on_buttonSub_clicked()
{
    income_l->removeObject(currentIndex);
    currentIndex = QModelIndex();
}

