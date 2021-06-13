#include "mainwindow.hh"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::onclose_button_pressed);
    connect(ui->weightLineEdit, &QLineEdit::editingFinished, this, &MainWindow::onweight_inputted);
    connect(ui->weightLineEdit, &QLineEdit::textChanged, this, &MainWindow::onweight_inputted);
    connect(ui->heightLineEdit, &QLineEdit::editingFinished, this, &MainWindow::onheight_inputted);
    connect(ui->heightLineEdit, &QLineEdit::textChanged, this, &MainWindow::onheight_inputted);
    connect(ui->countButton, &QPushButton::clicked, this, &MainWindow::oncount_button_pressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onclose_button_pressed()
{
    close();
}

void MainWindow::onweight_inputted()
{
    QString weight_as_string = ui->weightLineEdit->text();
    weight_ = weight_as_string.toInt();
}

void MainWindow::onheight_inputted()
{
    QString height_as_string = ui->heightLineEdit->text();
    height_ = height_as_string.toInt();
}

void MainWindow::oncount_button_pressed()
{
    double height_in_m = height_ / 100;
    double result = (weight_ / (height_in_m * height_in_m));
    QString result_as_qstring = QString::number(result);

    ui->resultLabel->setText(result_as_qstring);

    if (result < 18.5)
    {
        ui->infoTextBrowser->setText(UNDERWEIGHT);
    }
    else if (result > 25)
    {
        ui->infoTextBrowser->setText(OVERWEIGHT);
    }
    else
    {
        ui->infoTextBrowser->setText(NORMAL);
    }
}

