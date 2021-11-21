#ifndef CAREAMANAGER_H
#define CAREAMANAGER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "ccell.h"
#include "cvisualblock.h"

// TODO: на будущее - увеличить шрифт?

class CAreaManager : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CAreaManager(QWidget *parent = nullptr);
    ~CAreaManager();
    void init();

    //    void test();
    //! Добавить на сцену блоки со словами в пул слов (внизу)
    void addWords(QStringList words);

public slots:
    void blockMoved();

private:
    void _buildMatrix();

    //! Ищем свободное место для блока
    QPoint _findFreePlaceForBlock(quint32 width);

private:
    QGraphicsScene *m_scene;
    QList<QList<CCell *> *> m_matrix;
    QList<CVisualBlock *> m_vBlocks;
    //! Высота с которой начинается зона пула слов
    qint32 m_poolStarts;
};

#endif // CAREAMANAGER_H
