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
    //! Делаем клетку занятой и записываем в неё id слова, которое на ней написано
    void setBusy(bool st, qint32 wordId = 0);
    bool isBusy() { return m_busy; }
    //! Возвращает id слова, которое на ней написано
    qint32 getWordId() { return m_wordId; }

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QString m_id { "0" };
    bool m_busy { false };
    qint32 m_wordId;
};

#endif // CCELL_H
