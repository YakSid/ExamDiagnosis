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
    // TODO: добавил tooltip, чтобы в несколько строк появлялся текст
    Q_OBJECT
public:
    explicit CVisualBlock(QObject *parent = nullptr);
    CVisualBlock(QString text, QFont font, QObject *parent = nullptr);
    ~CVisualBlock();
    void setState(EBlockState state);
    quint32 getWidth() { return m_width; }
    //! Перемещение на ближайшую свободную позицию на сцене
    bool goOnFreePlaceOnScene();
    //! Мой метод, отличающийся от классического использованием busyCells
    void setPosition(const QPointF &pos);

signals:
    void s_blockMoved();

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
    //! Сделать новые клетки занятыми, записать, а старые выписать и сделать свободными
    void _changeBusyCells();

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
