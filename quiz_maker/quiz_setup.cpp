#include "quiz_setup.h"
#include "ui_quiz_setup.h"

quiz_setup::quiz_setup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::quiz_setup),
    qp(new quiz_play())
{
    ui->setupUi(this);

    connect(ui->comp_setup, &QPushButton::clicked, this, [this](){
        this->hide();
        qp->show();
    });
}

quiz_setup::~quiz_setup()
{
    delete ui;
}
