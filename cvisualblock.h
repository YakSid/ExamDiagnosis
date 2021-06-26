#ifndef CVISUALBLOCK_H
#define CVISUALBLOCK_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>

class CVisualBlock : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CVisualBlock(QObject *parent = nullptr);
    ~CVisualBlock();

signals:
public slots:

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CVISUALBLOCK_H
