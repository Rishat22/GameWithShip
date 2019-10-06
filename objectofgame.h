#ifndef OBJECT_OF_GAME_H
#define OBJECT_OF_GAME_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimer>

class ObjectOfGame : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit ObjectOfGame(QString imagePuth, QObject *parent = 0);
    virtual ~ObjectOfGame();

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QImage image_;
    QRect rectImage_;
};
class Ship : public ObjectOfGame
{
public:
    Ship (QString imagePuth, QObject *parent = 0);
    void moveTo(QPoint endPoint);
    QTimer* timer_;
    bool isSailing() const;

private:
    QPoint endPoint_;
    bool isSailing_;
};


#endif // OBJECT_OF_GAME_H
