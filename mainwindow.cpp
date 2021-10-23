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

    switch (m_menu->getMode()) {
    default:
    case 0:
        exit(0);
    case 1:
        break;
    }

    auto example = CJsonManager::loadFromFile(m_menu->getPathToSelectedTest());

    ui->setupUi(this);
    m_areaMg = new CAreaManager();
    ui->groupBox->layout()->addWidget(m_areaMg);
    m_areaMg->setRenderHint(QPainter::Antialiasing);
    m_areaMg->setCacheMode(QGraphicsView::CacheBackground);
    m_areaMg->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_areaMg->setSceneRect(0, 0, 950, 650);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // BUG: почему-то если в конструкторе, то неправильно считывает размер
    m_areaMg->init();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->setCursor(Qt::WaitCursor);
    m_areaMg->test();
    this->setCursor(Qt::ArrowCursor);
}

void MainWindow::on_pushButton_3_clicked()
{
    m_areaMg->test2();
}
