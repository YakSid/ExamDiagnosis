#include "careamanager.h"
#include <QDateTime>
#include <random>
#include <QMessageBox>

CAreaManager::CAreaManager(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene();
    setScene(m_scene);

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

CAreaManager::~CAreaManager()
{
    qDeleteAll(m_matrix);
}

void CAreaManager::init()
{
    _buildMatrix();

    //Углы
    /*m_scene->addRect(0, 0, 10, 10, QPen(Qt::blue), QBrush(Qt::yellow));
    m_scene->addRect(0, SCENE_HEIGHT - 10, 10, 10, QPen(Qt::blue), QBrush(Qt::green));
    m_scene->addRect(SCENE_WIDTH - 10, 0, 10, 10, QPen(Qt::blue), QBrush(Qt::gray));
    m_scene->addRect(SCENE_WIDTH - 10, SCENE_HEIGHT - 10, 10, 10, QPen(Qt::blue), QBrush(Qt::red));*/

    //Блоки заболеваний
    QFont disFont("times", 11);
    disFont.setItalic(true);
    for (int i = 0; i <= DIS_BLOCK.count(); i++) {
        if (i == DIS_BLOCK.count()) {
            //Добавление зон для слов
            m_poolStarts = i * (5 * (BLOCK_HEIGHT + YMARGIN_BETWEEN_CELLS)) - 2;
            m_scene->addLine(0, m_poolStarts, SCENE_WIDTH, m_poolStarts);
        } else {
            auto text = m_scene->addText(DIS_BLOCK.at(i), disFont);
            text->setPos(0, i * (5 * (BLOCK_HEIGHT + YMARGIN_BETWEEN_CELLS)) - 3);
            // Добавить занятость клеткам с текстом
            for (auto item : text->collidingItems()) {
                auto cell = static_cast<CCell *>(item);
                cell->setBusy(true, -(i + 1));
            }
        }
    }
}

void CAreaManager::addWords(QList<SWord *> words)
{
    QFont wordsFont("times", 12);
    //Случайная генерация основанная на Вихре Мерсенна
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int<int> ui(0, words.count() - 1);
    for (int i = 0; i < words.count() - 1; i++) {
        words.swap(ui(gen), ui(gen));
    }

    for (int i = 0; i < words.count(); i++) {
        auto vBlock = new CVisualBlock(words.at(i)->id, words.at(i)->text, wordsFont, m_matrix, this);
        m_vBlocks.append(vBlock);
        m_scene->addItem(vBlock);
        auto pos = _findFreePlaceForBlock(vBlock->getWidth());
        vBlock->setPosition(pos);
    }
}

void CAreaManager::summarize(const QStringList &combinations)
{
    //"1-2-3.4-5-6.7-8.9"
    auto combination = combinations.first(); // TODO: сделать для нескольких комбинаций
    // NOTE: Удаление последней точки, если она есть (она всегда есть)
    if (combination.right(1) == ".")
        combination.chop(1);

    //Переводим правильную комбинацию в список правильных порядков
    QList<QList<qint32>> rightCombination = _getRightOrderCombination(combination);

    //Переводим нашу комбинацию в список наших порядков
    QList<QList<qint32>> ourCombination = _getCurrentOrderCombination();

    //Сверка получившегося с необходимым, окрашивание
    for (int block = 0; block < 5; block++) {
        if (block == 3) {
            //Проверка 3 (последнего) блока (тут не важен порядок слов)
            QList<qint32> rightOrderInBlock = rightCombination.at(block);
            QList<qint32> ourOrderInBlock = ourCombination.at(block);

            for (auto num : ourOrderInBlock) {
                if (rightOrderInBlock.contains(num)) {
                    _setBlockState(num, EBlockState::noDifference);
                } else {
                    _setBlockState(num, EBlockState::incorrect);
                }
            }
        } else if (block == 4) {
            //Проверка неиспользованных слов (если есть 4 блок в ourCombinations)
            if (ourCombination.length() > 4) {
                if (!ourCombination.at(4).isEmpty())
                    for (auto num : ourCombination.at(4))
                        _setBlockState(ourCombination.at(4).at(num), EBlockState::incorrect);
                QMessageBox msg;
                msg.setText("Вы использовали не все блоки");
                msg.exec();
            }
        } else {
            //Проверка 0,1,2 блоков
            QList<qint32> rightOrderInBlock = rightCombination.at(block);
            QList<qint32> ourOrderInBlock = ourCombination.at(block);

            if (ourOrderInBlock.isEmpty())
                continue;

            //Избегаем ошибки разницы количества слов
            if (rightOrderInBlock.length() < ourOrderInBlock.length()) {
                ///Если наших слов больше, то это ошибка - для сверки дополняем правильную комбинацию
                ///по-любому-ошибочными вариантами, чтобы окрасить наши в красный за несовпадение - по факту за неверный
                ///блок
                qint32 countOfWrongWords = ourOrderInBlock.length() - rightOrderInBlock.length();
                for (int i = 0; i < countOfWrongWords; i++) {
                    rightOrderInBlock += -10;
                }
            }

            //Сверяем все слова в блоке верном и имеющемся
            for (int i = 0; i < ourOrderInBlock.length(); i++) {
                if (rightOrderInBlock.at(i) == ourOrderInBlock.at(i)) {
                    _setBlockState(ourOrderInBlock.at(i), EBlockState::correct);
                } else {
                    _setBlockState(ourOrderInBlock.at(i), EBlockState::incorrect);
                }
            }
        }
    }
}

void CAreaManager::clearArea()
{
    for (auto block : m_vBlocks) {
        block->setCellsFree();
        delete block;
    }
    m_vBlocks.clear();
}

void CAreaManager::_buildMatrix()
{
    auto cellsInLine = this->sceneRect().width() / (BLOCK_WIDTH + XMARGIN_BETWEEN_CELLS);
    auto cellsInRow = (this->sceneRect().height() / (BLOCK_HEIGHT + YMARGIN_BETWEEN_CELLS)) - 1;
    for (int j = 0; j < cellsInRow; j++) {
        auto line = new QList<CCell *>;
        for (int i = 0; i < cellsInLine; i++) {
            auto cell = new CCell(QString::number(i) + "-" + QString::number(j));
            cell->setPos(i * (BLOCK_WIDTH + XMARGIN_BETWEEN_CELLS), j * (BLOCK_HEIGHT + YMARGIN_BETWEEN_CELLS));
            m_scene->addItem(cell);
            line->append(cell);
        }
        m_matrix.append(line);
    }
}

QPoint CAreaManager::_findFreePlaceForBlock(quint32 width)
{
    //! Сколько нужно клеток для слова
    quint32 needCellsCount = width / (BLOCK_WIDTH + XMARGIN_BETWEEN_CELLS) + 2;
    //! Сколько свободных клеток подряд нашли
    quint32 foundFreeCells = 0;

    for (auto line : m_matrix) {
        //Если ниже пула слов, то работаем с линией
        if (line->first()->y() > m_poolStarts) {
            //Проход по каждой линии в пуле слов
            foundFreeCells = 0;
            for (auto cell : *line) {
                if (cell->isBusy()) {
                    foundFreeCells = 0;
                } else {
                    foundFreeCells++;
                }

                if (foundFreeCells == needCellsCount) {
                    //Возвращаем точку первой клетки
                    return QPoint(cell->x() - width, cell->y());
                }
            }
        }
    }

    return QPoint();
}

void CAreaManager::_setBlockState(qint32 wordId, EBlockState state)
{
    for (auto block : m_vBlocks) {
        if (block->getWordId() == wordId) {
            block->setState(state);
            return;
        }
    }
}

QList<QList<qint32>> CAreaManager::_getRightOrderCombination(const QString &combination)
{
    QList<QList<qint32>> rightCombination;
    auto blocks = combination.split('.');
    for (auto block : blocks) {
        auto numbersInBlock = block.split('-', QString::SkipEmptyParts);
        QList<qint32> rightCombInBlock;
        for (auto num : numbersInBlock) {
            rightCombInBlock.append(num.toInt());
        }
        rightCombination.append(rightCombInBlock);
    }
    return rightCombination;
}

QList<QList<qint32>> CAreaManager::_getCurrentOrderCombination()
{
    QList<QList<qint32>> order;
    QList<qint32> orderOldStyle = _getCurrentOrderOldStyle();

    /// TODO: чуть потом, но надо: рефакторинг, чтобы убрать костыль и тогда заработает окраска при полном незаполнении
    /// (сейчас 4блочные показывают серый цвет вемсто красного т.к. количество блоков здесь получается 4, а не 5, как
    /// обычно

    //Перевод комбинации из старого стиля в новый
    auto currentOrderInBlock = new QList<qint32>;
    for (int i = 0; i < orderOldStyle.length(); i++) {
        if (orderOldStyle[i] > 0) {
            //число - записываем в текущий порядок
            currentOrderInBlock->append(orderOldStyle[i]);
        } else {
            //разделитель - записываем существующий и создаём новый
            order.append(*currentOrderInBlock);
            delete currentOrderInBlock;
            currentOrderInBlock = new QList<qint32>;
        }
    }
    order.append(*currentOrderInBlock);
    delete currentOrderInBlock;

    //Костыль, который удаляет первый лист т.к. он пустой т.к. здесь я не учитываю -1 разделитель
    if (order.first().isEmpty())
        order.takeFirst();

    return order;
}

QList<qint32> CAreaManager::_getCurrentOrderOldStyle()
{
    QList<qint32> order;
    for (auto line : m_matrix) {
        for (auto cell : *line) {
            if (cell->isBusy())
                order += cell->getWordId();
        }
    }
    //Чистим от дубликатов с конца
    for (int i = order.length() - 1; i > 0; i--) {
        //Если order[i] < 0 значит это разделитель блоков
        if (order[i - 1] == order[i]) {
            order.removeAt(i);
        }
    }
    return order;
}
