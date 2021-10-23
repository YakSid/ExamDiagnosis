#include "careamanager.h"
#include <QDateTime>
#include <random>

CAreaManager::CAreaManager(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene();
    setScene(m_scene);

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

CAreaManager::~CAreaManager() {}

// TODO: выделять над каким блоком сейчас слово и если отпустить мышь, то поставить на ближайшую свободную позицию в
// этот блок

// TODO: при проверке ответов немодальным окном открывать правильные варианты ответа

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
            //Добавление пула слов и зоны для них
            m_poolStarts = i * (5 * (BLOCK_HEIGHT + YMARGIN_BETWEEN_CELLS));
            m_scene->addLine(0, m_poolStarts - 5, SCENE_WIDTH, m_poolStarts - 5);
        } else {
            auto text = m_scene->addText(DIS_BLOCK.at(i), disFont);
            text->setPos(0, i * (5 * (BLOCK_HEIGHT + YMARGIN_BETWEEN_CELLS)) - 5);
        }
    }
}

void CAreaManager::test()
{
    QFont wordsFont("times", 10);
    QStringList words = {
        "Ишемическая болезнь сердца",
        "Крупноочаговый кардиосклероз нижней стенки левого желудочка",
        "Стенозирующий атеросклероз коронарных артерий (3 степень 4 стадия стеноз до 50%)",
        "Гипертоническая болезнь: гипертрофия миокарда преимущественно левого желудочка (масса сердца – 570 г, толщина "
        "миокарда левого желудочка – 1,9 см, правого – 0,3 см, межжелудочковой перегородки – 1,9 см)",
        "Сахарный диабет 2 типа",
        "Диабетическая ангиопатия",
        "Диабетическая нефропатия (см. также клинические данные)",
        "Ожирение",
        "Застойная сердечная недостаточность",
        "«Мускатная» печень",
        "Цианотическая индурация селезенки и почек",
        "Двусторонний гидроторакс (1400 мл)",
        "Гидроперикард (200 мл)",
        "Асцит (8000 мл)",
        "Отёки подкожной клетчатки туловища и конечностей",
        "Дистрофические изменения внутренних органов и острое венозное полнокровие",
        "Отёк лёгких",
        "Атеросклероз аорты (3 степень, 4 стадия)",
        "Хронический панкреатит, ремиссия",
    };

    //Случайная генерация основанная на Вихре Мерсенна
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int<int> ui(0, words.count() - 1);
    for (int i = 0; i < words.count() - 1; i++) {
        words.swap(ui(gen), ui(gen));
    }

    for (int i = 0; i < words.count(); i++) {
        auto vBlock = new CVisualBlock(words.at(i), wordsFont);
        m_vBlocks.append(vBlock);
        m_scene->addItem(vBlock);
        vBlock->setPos(0, m_poolStarts);
        vBlock->goOnFreePlaceOnScene();
        connect(vBlock, &CVisualBlock::s_blockMoved, this, &CAreaManager::blockMoved);
    }
}

void CAreaManager::test2()
{
    // TODO: рассчитать на правильной позиции ли стоит блок
    ///Пока что на рандом раскрашиваю для проверки
    qint32 i = 0;
    for (auto vBlock : m_vBlocks) {
        switch (i) {
        case 0:
            vBlock->setState(EBlockState::correct);
            break;
        case 1:
            vBlock->setState(EBlockState::incorrect);
            break;
        case 2:
            vBlock->setState(EBlockState::noDifference);
            break;
        }

        i++;
        if (i > 2)
            i = 0;
    }
}

void CAreaManager::blockMoved()
{
    // TODO: Сдвинуть все блоки влево и вверх если освободилось место
}

void CAreaManager::_buildMatrix()
{
    auto cellsInLine = this->width() / (BLOCK_WIDTH + XMARGIN_BETWEEN_CELLS);
    auto cellsInRow = this->height() / (BLOCK_HEIGHT + YMARGIN_BETWEEN_CELLS);
    for (int i = 0; i < cellsInLine; i++) {
        for (int j = 0; j < cellsInRow; j++) {
            auto cell = new CCell();
            cell->setPos(i * (BLOCK_WIDTH + XMARGIN_BETWEEN_CELLS), j * (BLOCK_HEIGHT + YMARGIN_BETWEEN_CELLS));
            m_scene->addItem(cell);
        }
    }
}
