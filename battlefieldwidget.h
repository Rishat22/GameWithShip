#ifndef BATTLEFIELDWIDGET_H
#define BATTLEFIELDWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <algorithm>
//-----------
#include "objectofgame.h"
#include "battlefieldview.h"
#include "pointmatrix.h"
#include "endgamewidget.h"

namespace Ui {
class BattlefieldWidget;
}

class BattlefieldWidget : public QWidget
{
    Q_OBJECT
private:
    void beginningMoveShip();
    QVector<Index> routeCalculation();
public:
    explicit BattlefieldWidget(QWidget *parent = 0);
    QSharedPointer<Index> curShipIndex_;
    ~BattlefieldWidget();
private:
    Ui::BattlefieldWidget *ui;
    QSharedPointer<QGraphicsScene> scene_;
    QSharedPointer<BattlefieldView> view_;
    QScopedPointer<PointMatrix> pointMatrix_; //same as a unique_pointer
    Ship* ship_;
    QScopedPointer<EndGameWidget> endGame_;
private slots:
    void slotAddMine(QPoint mousePoint);
    void startGame();
};

#endif // BATTLEFIELDWIDGET_H
