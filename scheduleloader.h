#ifndef SCHEDULERLOADER_H
#define SCHEDULERLOADER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QJsonObject>

struct Lesson {
    QString time;
    QString subject;
    QString teacher;
    QString group;
    QString room;
    QString building;  // ← добавь эту строку
};

using DaySchedule = QMap<QString, QVector<Lesson>>;

struct WeekSchedule {
    DaySchedule odd;
    DaySchedule even;
};

struct TeacherData {
    QString name;
    WeekSchedule schedule;
};

struct GroupData {
    QString name;
    WeekSchedule schedule;
};

class ScheduleLoader
{
public:
    bool load(const QString &filePath);

    QVector<TeacherData> teachers;
    QVector<GroupData>   groups;

private:
    DaySchedule parseDaySchedule(const QJsonObject &obj, bool isTeacher);
};

#endif // SCHEDULERLOADER_H