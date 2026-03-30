#include "quiz_play.h"
#include "ui_quiz_play.h"

void update_label(QString q, int i);

quiz_play::quiz_play(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::quiz_play)
    , timer(new QTimer(this))
    , quiz("問題文を制作するにあたって、１番時間がかかることは何でしょうか？")
    , read(0)
{
    ui->setupUi(this);

    connect(timer, &QTimer::timeout, this, [this]() {
        if (read < quiz.size()) {
            ui->quiz_text->setText(ui->quiz_text->text() + quiz[read]);
            read++;
        } else {
            timer->stop();
        }
    });
}

void quiz_play::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    ui->quiz_text->clear();
    read = 0;
    timer->start(100);
}

quiz_play::~quiz_play()
{
    delete ui;
}
