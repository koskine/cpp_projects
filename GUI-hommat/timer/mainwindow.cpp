#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    timer(new QTimer(this)),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onstart_button_clicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::onstop_button_clicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::onreset_button_clicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::onclose_button_clicked);

    connect(timer, &QTimer::timeout, this, &MainWindow::ontimer_timeout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ontimer_timeout()
{
    int current_mins = ui->lcdNumberMin->intValue();
    int current_secs = ui->lcdNumberSec->intValue();

    if (current_secs == 59)
    {
        ui->lcdNumberMin->display(current_mins + 1);
        ui->lcdNumberSec->display(0);
    }
    else
    {
        ui->lcdNumberSec->display(current_secs + 1);
    }
}

void MainWindow::onstart_button_clicked()
{
    timer->start(1000);
}

void MainWindow::onstop_button_clicked()
{
    timer->stop();
}

void MainWindow::onreset_button_clicked()
{
    ui->lcdNumberMin->display(0);
    ui->lcdNumberSec->display(0);
}

void MainWindow::onclose_button_clicked()
{
    close();
}
