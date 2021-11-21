#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ccrypter.h"
#include <QDebug>
#include <QCursor>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    m_menu = new CMenu();
    m_menu->setModal(true);
    m_menu->exec();

    if (m_menu->getMode() != 1)
        exit(0);
    auto example = CJsonManager::loadFromFile(m_menu->getPathToSelectedTest());

    ui->setupUi(this);
    m_areaMg = new CAreaManager();
    ui->groupBox->layout()->addWidget(m_areaMg);
    m_areaMg->setRenderHint(QPainter::Antialiasing);
    m_areaMg->setCacheMode(QGraphicsView::CacheBackground);
    m_areaMg->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_areaMg->setSceneRect(0, 0, 950, 650);
    m_areaMg->init();

    // NOTE: фикс бага writer программы
    QList<qint32> usedIds;
    //! Минимальный id используемый в комбинациях (другие слова оказались мусором)
    qint32 minId = example.words.count();
    auto comb = example.combinations.first();
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

    QStringList words;
    for (auto word : example.words) {
        if (word->id >= minId)
            if (usedIds.contains(word->id))
                words.append(word->text);
    }

    m_areaMg->addWords(words); // NOTE: если будет удобнее, то добавлять вместе с ID
    // NOTE: нижнюю строчку потом заменить на верхнюю
    //     m_areaMg->test();

    // TODO: 2. Переименовать клавиши из ui. По нажатию сделать проверку см. note
    // TODO: 3. Показать правильные возможные результаты (немодальным окном, в геометрии указав открыть слева от окна)
    //Возможно окрашивать только неверные в красный, а остальные оставлять?
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_checkAnswer_clicked() {}
