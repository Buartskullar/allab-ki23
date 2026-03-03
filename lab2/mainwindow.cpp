#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    income_l = new incomeList(this);
    ui->mainList->setModel(income_l);
    loadData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadData() {
    // 1. Создаем объект файла
    QFile file("C:/CODE/OSNOV/lab2/data.txt");

    // 2. Пытаемся открыть файл только для чтения и как текстовый
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    // 3. Используем поток для удобного чтения
    QTextStream in(&file);

    // 4. Читаем файл построчно до самого конца
    while (!in.atEnd()) {
        QString line = in.readLine();

        // Пропускаем пустые строки, если они есть
        if (line.trimmed().isEmpty()) {
            continue;
        }

        incops::income toAdd = incops::transformToIncome(line);
        income_l->addObject(toAdd);

        // --- ВАШ КОД БУДЕТ ЗДЕСЬ ---
        // Например:
        // IncomeItem item = IncomeItem::fromString(line);
        // m_model->addObject(item);
        // ---------------------------
    }

    // 5. Закрываем файл
    file.close();
}
