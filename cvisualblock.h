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
    CVisualBlock(qint32 wordId, QString text, QFont font, const QList<QList<CCell *> *> &matrix,
                 QObject *parent = nullptr);
    ~CVisualBlock();
    void setState(EBlockState state);
    quint32 getWidth() { return m_width; }
    //! Мой метод, отличающийся от классического использованием busyCells
    void setPosition(const QPointF &pos);
    //! Вернуть номер слова, которое записано в блоке
    qint32 getWordId() { return m_wordId; }

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
    //! Найти свободное место правее и если нужно ниже
    QPoint _findFreePlace();
    //! Выровнять расположение блока относительно сетки
    void _align(const QPointF &pos);

private:
    //! id слова, которое отображает этот блок
    qint32 m_wordId { -1 };
    //! Состояние блока в зависимости от расположения
    EBlockState m_state { EBlockState::none };

    QString m_text;
    quint32 m_width;
    QFont m_font;

    //! Позиция относительно виджета, где мы схватили его
    QPointF m_catchPos;
    //! Список клеток на которых расположен
    QList<CCell *> m_busyCells;
    //! Указатель на матрицу клеток, на которых распологаются объекты vBlock
    QList<QList<CCell *> *> m_matrix;
};

#endif // CVISUALBLOCK_H
