#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bool cleared = false;

    connect(ui->button_zero,  &QPushButton::clicked, this, [=](){ add_text("0"); });
    connect(ui->button_one,   &QPushButton::clicked, this, [=](){ add_text("1"); });
    connect(ui->button_two,   &QPushButton::clicked, this, [=](){ add_text("2"); });
    connect(ui->button_three, &QPushButton::clicked, this, [=](){ add_text("3"); });
    connect(ui->button_four,  &QPushButton::clicked, this, [=](){ add_text("4"); });
    connect(ui->button_five,  &QPushButton::clicked, this, [=](){ add_text("5"); });
    connect(ui->button_six,   &QPushButton::clicked, this, [=](){ add_text("6"); });
    connect(ui->button_seven, &QPushButton::clicked, this, [=](){ add_text("7"); });
    connect(ui->button_eight, &QPushButton::clicked, this, [=](){ add_text("8"); });
    connect(ui->button_nine,  &QPushButton::clicked, this, [=](){ add_text("9"); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_text(const QString &text)
{
    if(!cleared)
    {
        ui->display->clear();
        cleared = true;
    }
    ui->display->setText(ui->display->text() + text);
}
