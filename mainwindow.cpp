#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "teacherschedulewindow.h"
#include "studentschedulewindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Расписание кафедры");
    setFixedSize(300, 300);

    if (ui->comboWeek->count() == 0) {
        ui->comboWeek->addItem("Нечётная");
        ui->comboWeek->addItem("Чётная");
    }

    QString jsonPath = ":/data/schedule.json";
    qDebug() << "Загружаем JSON из ресурсов:" << jsonPath;

    if (!loader.load(jsonPath)) {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось загрузить расписание!\n" +
                                  QString("Искали: ") + jsonPath);
    } else {
        qDebug() << "Расписание успешно загружено!";
        qDebug() << "Групп:" << loader.groups.size();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpen_clicked()
{
    bool isOdd = (ui->comboWeek->currentIndex() == 0);

    if (ui->radioStudent->isChecked()) {
        if (loader.groups.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Нет данных о группах!");
            return;
        }

        StudentScheduleWindow *w = new StudentScheduleWindow(loader.groups, isOdd, this);
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();

    } else if (ui->radioTeacher->isChecked()) {
        if (loader.teachers.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Нет данных о преподавателях!");
            return;
        }

        TeacherScheduleWindow *w = new TeacherScheduleWindow(loader.teachers, isOdd, this);
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();

    } else {
        QMessageBox::warning(this, "Внимание", "Выберите тип расписания!");
    }
}