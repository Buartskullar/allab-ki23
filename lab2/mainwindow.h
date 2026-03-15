#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include "incomelist.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onListClicked(const QModelIndex &index);
    void updateName(const QString &name);
    void updateDay(const QString &day);
    void updatePlace(const QString &place);
    void updateSum(const QString &sum);


    void on_buttonAdd_clicked();
    void on_buttonSub_clicked();

private:
    Ui::MainWindow *ui;
    incomeList *income_l;
    QModelIndex currentIndex;



};
#endif // MAINWINDOW_H
