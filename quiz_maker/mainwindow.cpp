#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    setup(new quiz_setup)
{
    ui->setupUi(this);

    connect(ui->play_button, &QPushButton::clicked, this, [this](){
        setup->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
