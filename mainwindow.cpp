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

    QStringList words;
    for (auto word : example.words) {
        words.append(word->text);
    }
    // m_areaMg->addWords(words); // NOTE: если будет удобнее, то добавлять вместе с ID
    // NOTE: нижнюю строчку потом заменить на верхнюю
    m_areaMg->test();

    // TODO: 2. Переименовать клавиши из ui. По нажатию сделать проверку (m_area test 2) см. note
    // TODO: 4. Показать правильные возможные рузельтаты (немодальным окном, в геометрии указав открыть слева от окна)
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
    m_areaMg->test2();
}
