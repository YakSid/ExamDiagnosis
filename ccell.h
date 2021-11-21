#ifndef CCELL_H
#define CCELL_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "cstructs.h"

class CCell : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CCell(QString id, QObject *parent = nullptr);
    void setBusy(bool st);
    bool isBusy() { return m_busy; }

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QString m_id { "0" };
    bool m_busy { false };
};

#endif // CCELL_H
