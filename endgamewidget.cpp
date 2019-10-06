#include "endgamewidget.h"
#include "ui_endgamewidget.h"

EndGameWidget::EndGameWidget(bool isWon, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndGameWidget)
{
    ui->setupUi(this);
    if(isWon)
    {
        ui->endGameText->setText("Победа!");
        ui->restartButton->setVisible(false);
    }
    else
        ui->endGameText->setText("Поражение!");

    connect(ui->restartButton, &QPushButton::clicked, this, &EndGameWidget::close);
    connect(ui->restartButton, &QPushButton::clicked, this, &EndGameWidget::restartGame);
}

EndGameWidget::~EndGameWidget()
{
    delete ui;
}

void EndGameWidget::setEndGameText(QString endGameText)
{
    ui->endGameText->setText(endGameText);
}
