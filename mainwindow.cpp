#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->View->setScene(new Scene(ui->View));
}

MainWindow::~MainWindow()
{
    delete ui;
}

