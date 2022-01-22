#include "ccell.h"

CCell::CCell(QString id, QObject *parent) : QObject(parent), m_id(id) {}

void CCell::setBusy(bool st, qint32 wordId)
{
    m_busy = st;
    m_wordId = wordId;
    update();
}

QRectF CCell::boundingRect() const
{
    return QRectF(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT);
}

void CCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    Q_UNUSED(painter);
    /*painter->setPen(Qt::gray);
    painter->setRenderHint(QPainter::Antialiasing);
    if (m_busy) {
        painter->setBrush(Qt::blue);
    } else {
        painter->setBrush(Qt::white);
    }

    painter->drawRect(QRectF(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT));*/

    //    painter->setFont(QFont());
    //    painter->setBrush(Qt::black);
    // painter->drawText(QRect(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT), Qt::AlignCenter, m_id);
}
