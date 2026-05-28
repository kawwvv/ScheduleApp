#include "teacherschedulewindow.h"
#include "ui_teacherschedulewindow.h"
#include <QTableWidgetItem>
#include <QHeaderView>

TeacherScheduleWindow::TeacherScheduleWindow(const QVector<TeacherData> &teachers,
                                             bool isOddWeek,
                                             QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TeacherScheduleWindow)
    , m_teachers(teachers)
    , m_isOddWeek(isOddWeek)
{
    ui->setupUi(this);
    setWindowTitle("Расписание преподавателя");

    // Заполняем список преподавателей
    for (const TeacherData &teacher : m_teachers) {
        ui->comboTeacher->addItem(teacher.name);
    }

    // Настройка таблицы (5 колонок: без корпуса, у преподавателей нет здания)
    QStringList headers = {"День", "Время", "Дисциплина", "Группа", "Аудитория"};
    ui->tableSchedule->setColumnCount(headers.size());
    ui->tableSchedule->setHorizontalHeaderLabels(headers);

    // Настройка ширины колонок
    ui->tableSchedule->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableSchedule->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableSchedule->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableSchedule->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableSchedule->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    ui->tableSchedule->setAlternatingRowColors(true);

    // Показываем первого преподавателя
    if (!m_teachers.isEmpty()) {
        showSchedule(m_teachers[0]);
    }
}

TeacherScheduleWindow::~TeacherScheduleWindow()
{
    delete ui;
}

void TeacherScheduleWindow::on_comboTeacher_currentIndexChanged(int index)
{
    if (index >= 0 && index < m_teachers.size()) {
        showSchedule(m_teachers[index]);
    }
}

void TeacherScheduleWindow::showSchedule(const TeacherData &teacher)
{
    ui->tableSchedule->setRowCount(0);

    const DaySchedule &daySchedule = m_isOddWeek ? teacher.schedule.odd : teacher.schedule.even;

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
            if (ui->tableSchedule->item(row, 2)) {
                ui->tableSchedule->item(row, 2)->setTextAlignment(Qt::AlignCenter);
            }
        }
        else if (lessons.size() == 1) {
            const Lesson &lesson = lessons[0];
            int row = ui->tableSchedule->rowCount();
            ui->tableSchedule->insertRow(row);
            ui->tableSchedule->setItem(row, 0, new QTableWidgetItem(day));
            ui->tableSchedule->setItem(row, 1, new QTableWidgetItem(lesson.time));
            ui->tableSchedule->setItem(row, 2, new QTableWidgetItem(lesson.subject));
            ui->tableSchedule->setItem(row, 3, new QTableWidgetItem(lesson.group));
            ui->tableSchedule->setItem(row, 4, new QTableWidgetItem(lesson.room));
        }
        else {
            for (const Lesson &lesson : lessons) {
                int row = ui->tableSchedule->rowCount();
                ui->tableSchedule->insertRow(row);

                if (row == firstRow) {
                    ui->tableSchedule->setItem(row, 0, new QTableWidgetItem(day));
                }

                ui->tableSchedule->setItem(row, 1, new QTableWidgetItem(lesson.time));
                ui->tableSchedule->setItem(row, 2, new QTableWidgetItem(lesson.subject));
                ui->tableSchedule->setItem(row, 3, new QTableWidgetItem(lesson.group));
                ui->tableSchedule->setItem(row, 4, new QTableWidgetItem(lesson.room));
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