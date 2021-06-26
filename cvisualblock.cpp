#include "cvisualblock.h"

const qint32 BLOCK_HEIGHT = 30;

CVisualBlock::CVisualBlock(QObject *parent) : QObject(parent) {}

CVisualBlock::~CVisualBlock() {}

QRectF CVisualBlock::boundingRect() const
{
    return QRectF(-30, -30, 100, BLOCK_HEIGHT);
}

void CVisualBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(Qt::black);
    painter->setBrush(Qt::green);
    painter->drawRect(-30, -30, 100, BLOCK_HEIGHT);
}

void CVisualBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPos(mapToScene(event->pos()));
}

void CVisualBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    this->setCursor(QCursor(Qt::ClosedHandCursor));
}

void CVisualBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    this->setCursor(QCursor(Qt::ArrowCursor));
}
