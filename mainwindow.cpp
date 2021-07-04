#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ccrypter.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
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
    m_areaMg->test();
}

void MainWindow::on_pushButton_3_clicked()
{
    m_areaMg->test2();
}
