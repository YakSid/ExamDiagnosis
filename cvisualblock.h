#ifndef CVISUALBLOCK_H
#define CVISUALBLOCK_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include "ccell.h"

class CVisualBlock : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CVisualBlock(QObject *parent = nullptr);
    CVisualBlock(QString text, QFont font, QObject *parent = nullptr);
    ~CVisualBlock();
    void setState(EBlockState state);
    //! Перемещение на ближайшую свободную позицию на сцене
    bool goOnFreePlaceOnScene();

signals:
    void s_blockMoved();

public slots:

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    //! Рассчитать длинну блока необходмую для текста
    qint32 _countWidthForText();
    //! Создать градиент
    QLinearGradient _makeGradient(EBlockState state);

private:
    //! Состояние блока в зависимости от расположения
    EBlockState m_state { EBlockState::none };

    QString m_text;
    quint32 m_width;
    QFont m_font;

    //! Позиция относительно виджета, где мы схватили его
    QPointF m_catchPos;
    //! Список клеток на которых расположен
    QList<CCell *> m_busyCells;
};

#endif // CVISUALBLOCK_H
