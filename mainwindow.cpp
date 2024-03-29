#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ccrypter.h"
#include "cresult.h"
#include <QDebug>
#include <QCursor>

// TODO: деструкторы сделать, проверить
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    m_menu = new CMenu();
    m_menu->setModal(true);
    m_menu->exec();

    if (m_menu->getMode() != 1)
        exit(0);
    m_example = CJsonManager::loadFromFile(m_menu->getPathToSelectedTest());

    ui->setupUi(this);
    m_areaMg = new CAreaManager();
    ui->groupBox->layout()->addWidget(m_areaMg);
    m_areaMg->setRenderHint(QPainter::Antialiasing);
    m_areaMg->setCacheMode(QGraphicsView::CacheBackground);
    m_areaMg->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_areaMg->setSceneRect(0, 0, 950, 700);
    m_areaMg->init();

    m_areaMg->addWords(_findWordsFixWritersBug());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_checkAnswer_clicked()
{
    // Подвдодим итог - окрашиваем наши ответы в сигнализирующие цвета
    m_areaMg->summarize(m_example.combinations);

    //Отображаем окно с правильным результатами
    auto result = new CResult(this);
    result->init(m_example);
    result->setModal(false);
    result->exec();
}

void MainWindow::on_pb_back_clicked()
{
    m_areaMg->clearArea();

    m_menu->updateMode();
    m_menu->exec();

    if (m_menu->getMode() != 1)
        exit(0);
    m_example = CJsonManager::loadFromFile(m_menu->getPathToSelectedTest());

    m_areaMg->addWords(_findWordsFixWritersBug());
}

QList<SWord *> MainWindow::_findWordsFixWritersBug()
{
    QList<qint32> usedIds;
    //! Минимальный id используемый в комбинациях (другие слова оказались мусором)
    qint32 minId = m_example.words.count();
    auto comb = m_example.combinations.first();
    QString value; //Значение, которое будет сравниваться с минимальным
    //Проход по строке-комбинации и поиск минимального id
    for (int i = 0; i < comb.length(); i++) {
        //Если цифра, то приписываем к значению
        if (comb[i] != '-' && comb[i] != '.') {
            value += comb[i];
        } else if (!value.isEmpty()) {
            //Число собрано, производим сравнение
            if (value.toInt() < minId) {
                minId = value.toInt();
            }
            usedIds.append(value.toInt());
            value.clear();
        }
    }

    //! Отбираем нужные слова
    QList<SWord *> words;
    for (auto word : m_example.words) {
        if (word->id >= minId)
            if (usedIds.contains(word->id))
                words.append(word);
    }

    return words;
}
