#include "studentschedulewindow.h"
#include "ui_studentschedulewindow.h"
#include <QTableWidgetItem>
#include <QHeaderView>

StudentScheduleWindow::StudentScheduleWindow(const QVector<GroupData> &groups,
                                             bool isOddWeek,
                                             QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StudentScheduleWindow)
    , m_groups(groups)
    , m_isOddWeek(isOddWeek)
{
    ui->setupUi(this);
    setWindowTitle("Расписание студента");

    for (const GroupData &group : m_groups) {
        ui->comboGroup->addItem(group.name);
    }

    QStringList headers = {"День", "Время", "Дисциплина", "Преподаватель", "Аудитория", "Корпус"};
    ui->tableSchedule->setColumnCount(headers.size());
    ui->tableSchedule->setHorizontalHeaderLabels(headers);

    ui->tableSchedule->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableSchedule->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableSchedule->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableSchedule->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableSchedule->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableSchedule->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    ui->tableSchedule->setAlternatingRowColors(true);

    if (!m_groups.isEmpty()) {
        showSchedule(m_groups[0]);
    }
}

StudentScheduleWindow::~StudentScheduleWindow()
{
    delete ui;
}

void StudentScheduleWindow::on_comboGroup_currentIndexChanged(int index)
{
    if (index >= 0 && index < m_groups.size()) {
        showSchedule(m_groups[index]);
    }
}

void StudentScheduleWindow::showSchedule(const GroupData &group)
{
    ui->tableSchedule->setRowCount(0);

    const DaySchedule &daySchedule = m_isOddWeek ? group.schedule.odd : group.schedule.even;

    QStringList dayOrder = {"Понедельник", "Вторник", "Среда",
                            "Четверг", "Пятница", "Суббота"};

    for (const QString &day : dayOrder) {
        QVector<Lesson> lessons = daySchedule.value(day);
        int firstRow = ui->tableSchedule->rowCount();

        if (lessons.isEmpty()) {
            int row = ui->tableSchedule->rowCount();
            ui->tableSchedule->insertRow(row);
            ui->tableSchedule->setItem(row, 0, new QTableWidgetItem(day));
            ui->tableSchedule->setItem(row, 1, new QTableWidgetItem(""));
            ui->tableSchedule->setItem(row, 2, new QTableWidgetItem("Нет занятий"));
            ui->tableSchedule->setItem(row, 3, new QTableWidgetItem(""));
            ui->tableSchedule->setItem(row, 4, new QTableWidgetItem(""));
            ui->tableSchedule->setItem(row, 5, new QTableWidgetItem(""));
            if (ui->tableSchedule->item(row, 2)) {
                ui->tableSchedule->item(row, 2)->setTextAlignment(Qt::AlignCenter);
            }
        } else if (lessons.size() == 1) {
            const Lesson &lesson = lessons[0];
            int row = ui->tableSchedule->rowCount();
            ui->tableSchedule->insertRow(row);
            ui->tableSchedule->setItem(row, 0, new QTableWidgetItem(day));
            ui->tableSchedule->setItem(row, 1, new QTableWidgetItem(lesson.time));
            ui->tableSchedule->setItem(row, 2, new QTableWidgetItem(lesson.subject));
            ui->tableSchedule->setItem(row, 3, new QTableWidgetItem(lesson.teacher));
            ui->tableSchedule->setItem(row, 4, new QTableWidgetItem(lesson.room));
            ui->tableSchedule->setItem(row, 5, new QTableWidgetItem(lesson.building));
        } else {
            for (const Lesson &lesson : lessons) {
                int row = ui->tableSchedule->rowCount();
                ui->tableSchedule->insertRow(row);
                if (row == firstRow) {
                    ui->tableSchedule->setItem(row, 0, new QTableWidgetItem(day));
                }
                ui->tableSchedule->setItem(row, 1, new QTableWidgetItem(lesson.time));
                ui->tableSchedule->setItem(row, 2, new QTableWidgetItem(lesson.subject));
                ui->tableSchedule->setItem(row, 3, new QTableWidgetItem(lesson.teacher));
                ui->tableSchedule->setItem(row, 4, new QTableWidgetItem(lesson.room));
                ui->tableSchedule->setItem(row, 5, new QTableWidgetItem(lesson.building));
            }
            int lastRow = ui->tableSchedule->rowCount() - 1;
            if (lastRow > firstRow) {
                ui->tableSchedule->setSpan(firstRow, 0, lastRow - firstRow + 1, 1);
                if (ui->tableSchedule->item(firstRow, 0)) {
                    ui->tableSchedule->item(firstRow, 0)->setTextAlignment(Qt::AlignCenter);
                }
            }
        }
    }

    ui->tableSchedule->resizeColumnsToContents();
    ui->tableSchedule->resizeRowsToContents();
}