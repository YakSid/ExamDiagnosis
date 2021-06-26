#include "careamanager.h"
#include "cvisualblock.h"

CAreaManager::CAreaManager(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene();
    setScene(m_scene);

    // NOTE: Координатная разметка
    /*QPen penGreen(Qt::green);
    const qint32 LIMIT = 20;
    for (int i = -LIMIT; i < LIMIT; i++)
        for (int j = -LIMIT; j < LIMIT; j++) {
            QLineF line(i * 10, j * 10, i * 10, j * 10 - 1);
            m_scene->addLine(line, penGreen);
        }
    QLineF axisX(0, 0, 100, 0);
    m_scene->addLine(axisX, QPen(Qt::blue));
    QLineF axisY(0, -100, 0, 100);
    m_scene->addLine(axisY, QPen(Qt::blue));
    QLineF axisRed(60, -100, 60, 100);
    m_scene->addLine(axisRed, QPen(Qt::red));*/

    //
    for (int i = 0; i < 5; i++) {
        auto vBlock = new CVisualBlock();
        vBlock->setPos(i * 10, i * 10);
        m_scene->addItem(vBlock);
    }
}

CAreaManager::~CAreaManager() {}
