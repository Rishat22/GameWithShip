#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    battlefieldwidget_.reset(new BattlefieldWidget());
    ui->layout->addWidget(battlefieldwidget_.data());

    this->setWindowTitle("Game ship");

}
MainWindow::~MainWindow()
{
    delete ui;
}

