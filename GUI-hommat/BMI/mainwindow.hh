#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const QString UNDERWEIGHT = "You are underweight.";
const QString OVERWEIGHT = "You are overweight.";
const QString NORMAL = "Your weight is normal.";

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onclose_button_pressed();
    void onweight_inputted();
    void onheight_inputted();
    void oncount_button_pressed();

private:
    Ui::MainWindow *ui;
    double weight_;
    double height_;

};
#endif // MAINWINDOW_HH
