#include "ccell.h"

CCell::CCell(QObject *parent) : QObject(parent) {}

void CCell::setBusy(bool st)
{
    m_busy = st;
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
    //    painter->drawText(QRect(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT), Qt::AlignCenter, "text");
}
