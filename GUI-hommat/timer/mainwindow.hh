#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Normally this member should be in private part of the
    // class, but due to the automated tests it must be more
    // accessible.
    QTimer* timer;

private slots:
    void ontimer_timeout();
    void onstart_button_clicked();
    void onstop_button_clicked();
    void onreset_button_clicked();
    void onclose_button_clicked();

private:
    Ui::MainWindow *ui;
    // Add other necessary members here


};

#endif // MAINWINDOW_HH
