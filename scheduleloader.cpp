#include "scheduleloader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

bool ScheduleLoader::load(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Не удалось открыть файл:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qDebug() << "Ошибка парсинга JSON";
        return false;
    }

    QJsonObject root = doc.object();

    // Загрузка преподавателей
    QJsonArray teachersArray = root["teachers"].toArray();
    for (const QJsonValue &tVal : teachersArray) {
        QJsonObject t = tVal.toObject();
        TeacherData td;
        td.name = t["name"].toString();

        QJsonObject sched = t["schedule"].toObject();
        td.schedule.odd  = parseDaySchedule(sched["odd"].toObject(), true);
        td.schedule.even = parseDaySchedule(sched["even"].toObject(), true);
        teachers.append(td);
    }

    // Загрузка групп
    QJsonArray groupsArray = root["groups"].toArray();
    for (const QJsonValue &gVal : groupsArray) {
        QJsonObject g = gVal.toObject();
        GroupData gd;
        gd.name = g["name"].toString();

        QJsonObject sched = g["schedule"].toObject();
        gd.schedule.odd  = parseDaySchedule(sched["odd"].toObject(), false);
        gd.schedule.even = parseDaySchedule(sched["even"].toObject(), false);
        groups.append(gd);
    }

    qDebug() << "Загружено преподавателей:" << teachers.size();
    qDebug() << "Загружено групп:" << groups.size();

    return true;
}

DaySchedule ScheduleLoader::parseDaySchedule(const QJsonObject &obj, bool isTeacher)
{
    DaySchedule result;
    QStringList days = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};

    for (const QString &day : days) {
        QVector<Lesson> lessons;
        QJsonArray lessonsArray = obj[day].toArray();

        for (const QJsonValue &lVal : lessonsArray) {
            QJsonObject l = lVal.toObject();
            Lesson lesson;
            lesson.time    = l["time"].toString();
            lesson.subject = l["subject"].toString();
            lesson.room    = l["room"].toString();
            lesson.building = l["building"].toString();

            if (isTeacher) {
                lesson.group = l["group"].toString();
            } else {
                lesson.teacher = l["teacher"].toString();
            }

            lessons.append(lesson);
        }
        result[day] = lessons;
    }
    return result;
}
