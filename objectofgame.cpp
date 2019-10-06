#include "objectofgame.h"
ObjectOfGame::ObjectOfGame(QString imagePuth, QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    image_.load(imagePuth);
    rectImage_.setX(-(image_.size().width() / 2));
    rectImage_.setY(-(image_.size().height() / 2));
    rectImage_.setSize(image_.size());
}

ObjectOfGame::~ObjectOfGame()
{

}

QRectF ObjectOfGame::boundingRect() const
{
    return rectImage_;   /// Ограничиваем область, в которой лежит объект
}

void ObjectOfGame::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(rectImage_, image_);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

Ship::Ship(QString imagePuth, QObject *parent)
    :ObjectOfGame(imagePuth,parent)
{
    isSailing_ = false;
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, [=](){

        QPoint diff = endPoint_ - this->pos().toPoint();
        int normY = 0;
        int normX = 0;
        if(diff.x() != 0)
            normX = diff.x()/abs(diff.x());
        if(diff.y() != 0)
            normY = diff.y()/abs(diff.y());
        this->setPos(mapToScene(normX, normY));

        if(this->pos() == endPoint_)
        {
            isSailing_ = false;
            timer_->stop();
        }
    });
    timer_->setInterval(20);
}

void Ship::moveTo(QPoint endPoint)
{
    endPoint_ = endPoint;
    isSailing_ = true;
    timer_->start();
}

bool Ship::isSailing() const
{
    return isSailing_;
}
