#ifndef ENDGAMEWIDGET_H
#define ENDGAMEWIDGET_H

#include <QWidget>

namespace Ui {
class EndGameWidget;
}

class EndGameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EndGameWidget(bool isWon, QWidget *parent = 0);
    ~EndGameWidget();
    void setEndGameText(QString endGameText);
signals:
    void restartGame();
private:
    Ui::EndGameWidget *ui;
};

#endif // ENDGAMEWIDGET_H
