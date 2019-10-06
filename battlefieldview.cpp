#include "battlefieldview.h"

BattlefieldView::BattlefieldView(QString imagePath, QWidget *parent)
    : QGraphicsView(parent)
{
    reader = new QImageReader(imagePath);
    this->setMaximumSize(reader->size());
    this->setMinimumSize(reader->size());

    QString styleObj;
    styleObj = "border-image: url(" + reader->fileName() + ");"; // set background style
    this->setStyleSheet(styleObj);
}

BattlefieldView::~BattlefieldView()
{
    delete reader;
}

void BattlefieldView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->setCursor(Qt::PointingHandCursor);
        emit mouseClicked(event->pos());
    }
}

void BattlefieldView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        this->setCursor(Qt::ArrowCursor);
}


