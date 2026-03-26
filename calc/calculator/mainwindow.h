#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool cleared;
    void add_text(const QString &text); // QStringは重い処理だからコピーではなく参照させる
    void clear_text();
    void delete_text();
    void run_calc(std::string s);
};
#endif // MAINWINDOW_H
