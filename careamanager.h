#ifndef CAREAMANAGER_H
#define CAREAMANAGER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "ccell.h"
#include "cvisualblock.h"

class CAreaManager : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CAreaManager(QWidget *parent = nullptr);
    ~CAreaManager();
    void init();

    void test();
    //! Добавить на сцену блоки со словами в пул слов (внизу)
    void addWords(QStringList words);

    void test2();

signals:
public slots:
    void blockMoved();

private:
    void _buildMatrix();

private:
    QGraphicsScene *m_scene;
    QList<QList<CCell>> m_matrix;
    QList<CVisualBlock *> m_vBlocks;
    //! Высота с которой начинается зона пула слов
    qint32 m_poolStarts;
};

#endif // CAREAMANAGER_H
