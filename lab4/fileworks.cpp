#include "fileworks.h"

fileworks::fileworks() {
    status = isFileOk();
}

bool fileworks::isFileOk(){
    QFile file("C:/CODE/OSNOV/lab2/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    file.close();
    return true;
}

QList<income> fileworks::loadFromFile(){
    QList<income> m_data;
    QFile file("C:/CODE/OSNOV/lab2/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return m_data;

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    static QRegularExpression re("^(.*):(\\d{2}\\.\\d{2}\\.\\d{4})\\s+\"(.*)\"\\s+(-?\\d+)$");

    while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch match = re.match(line);

        if (match.hasMatch()) {
            income obj;
            obj.setName(match.captured(1));
            obj.setDay(match.captured(2));
            obj.setPlace(match.captured(3));
            obj.setSum(match.captured(4).toInt());

            m_data.append(obj);
        }
    }

    file.close();
    return m_data;
}

void fileworks::saveToFile(QList<income> m_data) {
    QFile file("C:/CODE/OSNOV/lab2/data.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    for (const income &item : m_data) {
        out << item.getName() << ":"
            << item.getDay() << " "
            <<"\"" << item.getPlace() << "\" "
            << item.getSum() << "\n";
    }
    file.close();
}

void fileworks::saveToFile(QList<income> m_data, QString filename){
    QString fullpath = "C:/CODE/OSNOV/lab2/" + filename;
    QFile file(fullpath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    for (const income &item : m_data) {
        out << item.getName() << ":"
            << item.getDay() << " "
            <<"\"" << item.getPlace() << "\" "
            << item.getSum() << "\n";
    }
    file.close();
}

int fileworks::comCount(){
    QFile file("C:/CODE/OSNOV/lab2/coms.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 0;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    int lineCount = 0;

    while (!in.atEnd()) {
        in.readLine();
        lineCount++;
    }

    file.close();
    return lineCount;
}

QString fileworks::getCommand(int line){
    QFile file("C:/CODE/OSNOV/lab2/coms.txt");
    if (!file.isOpen()){
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return 0;
        }
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    for (int i = 0; i < line; i++){
        in.readLine();
    }
    return in.readLine();

    if (file.atEnd()) {
        file.close();
    }
}
