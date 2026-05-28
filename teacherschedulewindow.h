#ifndef TEACHERSCHEDULEWINDOW_H
#define TEACHERSCHEDULEWINDOW_H

#include <QDialog>
#include <QVector>
#include "scheduleloader.h"

namespace Ui {
class TeacherScheduleWindow;
}

class TeacherScheduleWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TeacherScheduleWindow(const QVector<TeacherData> &teachers,
                                   bool isOddWeek,
                                   QWidget *parent = nullptr);
    ~TeacherScheduleWindow();

private slots:
    void on_comboTeacher_currentIndexChanged(int index);

private:
    void showSchedule(const TeacherData &teacher);

    Ui::TeacherScheduleWindow *ui;
    QVector<TeacherData> m_teachers;
    bool m_isOddWeek;
};

#endif // TEACHERSCHEDULEWINDOW_H