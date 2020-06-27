#include <editor/ConfigReader.h>

#include <QFile>

#include <QDebug>

void ConfigReader::readFromFile(const QString &fileName)
{
    auto file = QFile(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    auto fileToString = file.readAll();
    file.close();

    QJsonDocument d = QJsonDocument::fromJson(fileToString);
    QJsonObject sett2 = d.object();
    qWarning() << sett2;
    QJsonValue value = sett2.value(QString("appName"));
    qWarning() << value;
    QJsonObject item = value.toObject();
}
