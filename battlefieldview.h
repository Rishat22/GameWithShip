#ifndef BATTLEFIELDVIEW_H
#define BATTLEFIELDVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QObject>
#include <QImageReader>
#include <QMouseEvent>

class BattlefieldView : public QGraphicsView
{
    Q_OBJECT
public:
    BattlefieldView(QString imagePath, QWidget *parent = 0);
    ~BattlefieldView();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void mouseClicked(QPoint);
private:
    QImageReader* reader;

};

#endif // BATTLEFIELDVIEW_H
