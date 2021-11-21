#include "cvisualblock.h"
#include <QLinearGradient>
#include "cstructs.h"
#include <QDebug>

const qint32 ONE_WIDTH_MARGIN = 10; //Отступ с одной стороны

CVisualBlock::CVisualBlock(QObject *parent) : QObject(parent) {}

CVisualBlock::CVisualBlock(QString text, QFont font, QObject *parent) : QObject(parent)
{
    const qint32 blockLettersLimit = 35;
    if (text.count() > blockLettersLimit) {
        QString tooltipText = text;
        //Ищем с какого пробела обрезать текст для отображения
        qint32 spaceIndex = text.indexOf(' ', blockLettersLimit);
        //Если пробелов после лимита не найдено, то значит слов немного и можно не обрезать
        if (spaceIndex != -1) {
            //Отбрасываем лишнюю часть вместе с пробелом и записываем в m_text
            text.chop(text.size() - spaceIndex);
            text += "...";
            m_text = text;

            //Редактурем сохранённый тултип текст
            qint32 currentLine = 1;
            //! Ищем ли мы сейчас пробел, чтобы подменить его на перенос строки
            bool lookinfForSpace = false;
            //! Превышение длинны предыдущей строки над лимитом
            qint32 excessInPrevLine = 0;
            for (int i = 0; i < tooltipText.length(); i++) {
                if (lookinfForSpace) {
                    if (tooltipText.at(i) == ' ') {
                        tooltipText.replace(i, 1, '\n');
                        lookinfForSpace = false;
                        currentLine++;
                        excessInPrevLine = 0;
                    } else {
                        excessInPrevLine++;
                    }
                } else if (i > blockLettersLimit * currentLine + excessInPrevLine) {
                    lookinfForSpace = true;
                }
            }
            this->setToolTip(tooltipText);
        }
    }

    m_text = text;
    m_font = font;
    auto width = _countWidthForText();

    if (width > BLOCK_WIDTH) {
        m_width = static_cast<quint32>(width);
    } else {
        m_width = BLOCK_WIDTH;
    }
}

CVisualBlock::~CVisualBlock() {}

void CVisualBlock::setState(EBlockState state)
{
    if (m_state != state) {
        m_state = state;
        update();
    }
}

QRectF CVisualBlock::boundingRect() const
{
    return QRectF(0, 0, m_width, BLOCK_HEIGHT);
}

void CVisualBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(Qt::black);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(_makeGradient(m_state));
    painter->drawRoundedRect(QRectF(0, 0, m_width, BLOCK_HEIGHT), 6, 6, Qt::AbsoluteSize);

    painter->setFont(m_font);
    painter->setBrush(Qt::black);
    painter->drawText(QRect(0, 0, static_cast<qint32>(m_width), BLOCK_HEIGHT), Qt::AlignCenter, m_text);
}

void CVisualBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPos(mapToScene(event->pos().x() - m_catchPos.x(), event->pos().y() - m_catchPos.y()));
}

void CVisualBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    m_catchPos = event->pos();
    this->setCursor(QCursor(Qt::ClosedHandCursor));
}

void CVisualBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    this->setCursor(QCursor(Qt::ArrowCursor));

    //Ровнение по Y
    if (static_cast<qint32>(this->pos().y()) % ONE_HEIGHT > (ONE_HEIGHT / 2)) {
        this->setY(this->pos().y() + (ONE_HEIGHT - static_cast<qint32>(this->pos().y()) % ONE_HEIGHT));
    } else {
        this->setY(this->pos().y() - (static_cast<qint32>(this->pos().y()) % ONE_HEIGHT));
    }
    //Ровнение по X
    if (static_cast<qint32>(this->pos().x()) % ONE_WIDTH > (ONE_WIDTH / 2)) {
        this->setX(this->pos().x() + (ONE_WIDTH - static_cast<qint32>(this->pos().x()) % ONE_WIDTH));
    } else {
        this->setX(this->pos().x() - (static_cast<qint32>(this->pos().x()) % ONE_WIDTH));
    }

    // Автоперемещение на нужную позицию рядом //TODO: ПЕРЕДЕЛАТЬ!
    //    goOnFreePlaceOnScene();
    _changeBusyCells();
}

qint32 CVisualBlock::_countWidthForText()
{
    QFontMetrics fMetrics(m_font);
    return fMetrics.width(m_text) + 2 * ONE_WIDTH_MARGIN;
}

QLinearGradient CVisualBlock::_makeGradient(EBlockState state)
{
    QColor color;
    switch (state) {
    case EBlockState::none:
        color = "#ccffff";
        break;
    case EBlockState::correct:
        color = "#00ff00";
        break;
    case EBlockState::incorrect:
        color = "#ff2020";
        break;
    case EBlockState::noDifference:
        color = "#f5fffa";
        break;
    }

    QLinearGradient gradient(0, 0, m_width, BLOCK_HEIGHT);
    gradient.setColorAt(0.1, color);
    gradient.setColorAt(0.5, QColor("#eeffff"));
    gradient.setColorAt(0.9, color);
    return gradient;
}

void CVisualBlock::_changeBusyCells()
{
    //! Список клеток, которые стали занятыми
    QList<CCell *> collidingCells;
    for (auto item : collidingItems()) {
        auto cell = static_cast<CCell *>(item);
        collidingCells.append(cell);
        if (!m_busyCells.contains(cell)) {
            cell->setBusy(true);
        }
    }
    for (auto busyCell : m_busyCells) {
        if (!collidingCells.contains(busyCell)) {
            busyCell->setBusy(false);
        }
    }
    m_busyCells = collidingCells;
}

bool CVisualBlock::goOnFreePlaceOnScene()
{
    //    bool positionFound = false;
    //    QList<QGraphicsItem *> collItems;
    //    while (!positionFound) {
    //        collItems = collidingItems();
    //        positionFound = true;
    //        for (auto collItem : collItems) {
    //            auto cell = static_cast<CCell *>(collItem);
    //            if (cell) {
    //                if (cell->isBusy()) {
    //                    //Клетка занята ищем новое место
    //                    positionFound = false;
    //                    if (SCENE_WIDTH - m_width - this->pos().x() > 0) {
    //                        //Место справа есть
    //                        this->setX(this->pos().x() + ONE_WIDTH);
    //                        break;
    //                    } else {
    //                        //Места справа нет
    //                        this->setY(this->pos().y() + ONE_HEIGHT);
    //                        this->setX(0);
    //                        break;
    //                    }
    //                }
    //            }
    //        }
    //    }

    //    _changeBusyCells();

    //    // TODO: [block move] Обработать ситуацию, когда место на сцене заканчивается
    return true;
}

void CVisualBlock::setPosition(const QPointF &pos)
{
    //Ровнение по Y
    if (static_cast<qint32>(pos.y()) % ONE_HEIGHT > (ONE_HEIGHT / 2)) {
        this->setY(pos.y() + (ONE_HEIGHT - static_cast<qint32>(pos.y()) % ONE_HEIGHT));
    } else {
        this->setY(pos.y() - (static_cast<qint32>(pos.y()) % ONE_HEIGHT));
    }
    //Ровнение по X
    if (static_cast<qint32>(pos.x()) % ONE_WIDTH > (ONE_WIDTH / 2)) {
        this->setX(pos.x() + (ONE_WIDTH - static_cast<qint32>(pos.x()) % ONE_WIDTH));
    } else {
        this->setX(pos.x() - (static_cast<qint32>(pos.x()) % ONE_WIDTH));
    }

    _changeBusyCells();
}
