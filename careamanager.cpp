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
    QFont wordsFont("times", 10);
    //Случайная генерация основанная на Вихре Мерсенна
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int<int> ui(0, words.count() - 1);
    for (int i = 0; i < words.count() - 1; i++) {
        words.swap(ui(gen), ui(gen));
    }

    for (int i = 0; i < words.count(); i++) {
        auto vBlock = new CVisualBlock(words.at(i)->id, words.at(i)->text, wordsFont, m_matrix);
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

    //Переводим нашу комбинацию в порядковый лист
    QList<qint32> originalComb;
    auto blocks = combination.split('.', QString::SkipEmptyParts);
    qint32 blockCounter = -1;
    for (auto block : blocks) {
        originalComb.append(blockCounter);
        auto words = block.split('-', QString::SkipEmptyParts);
        for (auto word : words) {
            originalComb.append(word.toInt());
        }
        blockCounter--;
    }
    //Составляем список порядка слов сейчас
    //! Порядок слов сейчас
    QList<qint32> order;
    for (auto line : m_matrix) {
        for (auto cell : *line) {
            if (cell->isBusy())
                order += cell->getWordId();
        }
    }
    //Чистим от дубликатов с конца
    for (int i = order.length() - 1; i > 0; i--) {
        if (order[i - 1] == order[i]) {
            order.removeAt(i);
        }
    }

    if (originalComb.count() != order.count()) {
        QMessageBox msg;
        msg.setText("Используйте все слова");
        msg.exec();
        return;
    }

    // TODO: усовершенствовать алгоритм окрашивания
    for (int i = 0; i < order.count(); i++) {
        if (order[i] > 0) {
            if (order[i] == originalComb[i]) {
                _setBlockState(order[i], EBlockState::correct);
            } else {
                _setBlockState(order[i], EBlockState::incorrect);
            }
        }
    }

    //Показать правильные возможные результаты (немодальным окном, в геометрии указав открыть слева от окна)
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
