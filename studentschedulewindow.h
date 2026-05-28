#ifndef STUDENTSCHEDULEWINDOW_H
#define STUDENTSCHEDULEWINDOW_H

#include <QDialog>
#include <QVector>
#include "scheduleloader.h"

namespace Ui {
class StudentScheduleWindow;
}

class StudentScheduleWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StudentScheduleWindow(const QVector<GroupData> &groups,
                                   bool isOddWeek,
                                   QWidget *parent = nullptr);
    ~StudentScheduleWindow();

private slots:
    void on_comboGroup_currentIndexChanged(int index);

private:
    void showSchedule(const GroupData &group);

    Ui::StudentScheduleWindow *ui;
    QVector<GroupData> m_groups;
    bool m_isOddWeek;
private:
    void adjustWindowSize();  // ← добавить эту строку
};

#endif // STUDENTSCHEDULEWINDOW_H