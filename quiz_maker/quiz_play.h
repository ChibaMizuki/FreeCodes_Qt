#ifndef QUIZ_PLAY_H
#define QUIZ_PLAY_H

#include <QWidget>
#include <QTimer>
#include <QShowEvent>

namespace Ui {
class quiz_play;
}

class quiz_play : public QWidget
{
    Q_OBJECT

public:
    explicit quiz_play(QWidget *parent = nullptr);
    ~quiz_play();

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::quiz_play *ui;
    QTimer *timer;
    QString quiz;
    int read;
};

#endif // QUIZ_PLAY_H
