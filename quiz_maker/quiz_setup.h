#ifndef QUIZ_SETUP_H
#define QUIZ_SETUP_H

#include <QWidget>
#include "quiz_play.h"

namespace Ui {
class quiz_setup;
}

class quiz_setup : public QWidget
{
    Q_OBJECT

public:
    explicit quiz_setup(QWidget *parent = nullptr);
    ~quiz_setup();

private:
    Ui::quiz_setup *ui;
    quiz_play *qp;
};

#endif // QUIZ_SETUP_H
