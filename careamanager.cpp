#include "careamanager.h"
#include <QDateTime>
#include <random>

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

// TODO: [block move] выделять над каким блоком сейчас слово и если отпустить мышь, то поставить на ближайшую свободную
// позицию в этот блок

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
            // TODO: добавить занятость клеткам с текстом
            for (auto item : text->collidingItems()) {
                auto cell = static_cast<CCell *>(item);
                cell->setBusy(true);
            }
        }
    }
}

// void CAreaManager::test()
//{
//    QFont wordsFont("times", 10);
//    /*QStringList words = { "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",
//                          "10", "11", "12", "13", "14", "15", "16", "17", "18" };*/
//    QStringList words = {
//        "Ишемическая болезнь сердца",
//        "Крупноочаговый кардиосклероз нижней стенки левого желудочка",
//        "Стенозирующий атеросклероз коронарных артерий (3 степень 4 стадия стеноз до 50%)",
//        "Гипертоническая болезнь: гипертрофия миокарда преимущественно левого желудочка (масса сердца – 570 г, толщина
//        " "миокарда левого желудочка – 1,9 см, правого – 0,3 см, межжелудочковой перегородки – 1,9 см)", "Сахарный
//        диабет 2 типа", "Диабетическая ангиопатия", "Диабетическая нефропатия (см. также клинические данные)",
//        "Ожирение",
//        "Застойная сердечная недостаточность",
//        "«Мускатная» печень",
//        "Цианотическая индурация селезенки и почек",
//        "Двусторонний гидроторакс (1400 мл)",
//        "Гидроперикард (200 мл)",
//        "Асцит (8000 мл)",
//        "Отёки подкожной клетчатки туловища и конечностей",
//        "Дистрофические изменения внутренних органов и острое венозное полнокровие",
//        "Отёк лёгких",
//        "Атеросклероз аорты (3 степень, 4 стадия)",
//        "Хронический панкреатит, ремиссия",
//    };

//    //Случайная генерация основанная на Вихре Мерсенна
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_int<int> ui(0, words.count() - 1);
//    for (int i = 0; i < words.count() - 1; i++) {
//        words.swap(ui(gen), ui(gen));
//    }

//    /*for (int i = 0; i < words.count(); i++) {
//        auto vBlock = new CVisualBlock(words.at(i), wordsFont);
//        m_vBlocks.append(vBlock);
//        m_scene->addItem(vBlock);
//        vBlock->setPos(0, m_poolStarts);
//        vBlock->goOnFreePlaceOnScene();
//        connect(vBlock, &CVisualBlock::s_blockMoved, this, &CAreaManager::blockMoved);
//    }*/
//}

void CAreaManager::addWords(QStringList words)
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
        auto vBlock = new CVisualBlock(words.at(i), wordsFont, m_matrix);
        m_vBlocks.append(vBlock);
        m_scene->addItem(vBlock);
        auto pos = _findFreePlaceForBlock(vBlock->getWidth());
        vBlock->setPosition(pos);
        connect(vBlock, &CVisualBlock::s_blockMoved, this, &CAreaManager::blockMoved);
    }
}

void CAreaManager::blockMoved()
{
    // TODO: [block move] Сдвинуть все блоки влево и вверх если освободилось место
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
