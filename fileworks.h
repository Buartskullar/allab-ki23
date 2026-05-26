#ifndef FILEWORKS_H
#define FILEWORKS_H
#include <QRegularExpression>
#include <QFile>
#include "income.h"

//Амогуы

class fileworks
{
private:
    bool isFileOk();
public:
    fileworks();
    bool status;

    QList<income> static loadFromFile();
    void static saveToFile(QList<income> m_data);
    void static saveToFile(QList<income> m_data, QString filename);
    int static comCount();
    QString static getCommand(int lineNum);
};

#endif // FILEWORKS_H
