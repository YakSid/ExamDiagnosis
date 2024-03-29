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
    //! Построить матрицу, установить надписи
    void init();
    //! Добавить на сцену блоки со словами в пул слов (внизу)
    void addWords(QList<SWord *> words);
    //! Составить получившуюся комбинацию слов и окрасить верные/неверные позиции
    void summarize(const QStringList &combinations);
    //! Подготовить для следующего теста - обнулить
    void clearArea();

private:
    void _buildMatrix();

    //! Ищем свободное место для блока
    QPoint _findFreePlaceForBlock(quint32 width);
    //! Найти в списке у какого блока это слово и сделать ему нужный цвет
    void _setBlockState(qint32 wordId, EBlockState state);

    //---Вычисления результата---
    //! Переводим правильную комбинацию в список правильных порядков
    QList<QList<qint32>> _getRightOrderCombination(const QString &combination);
    //! Получить текущий порядок слов
    QList<QList<qint32>> _getCurrentOrderCombination();
    //! Получить текущий порядок слов в старом стиле
    QList<qint32> _getCurrentOrderOldStyle();
    //---------------------------

private:
    QGraphicsScene *m_scene;
    QList<QList<CCell *> *> m_matrix;
    QList<CVisualBlock *> m_vBlocks;
    //! Высота с которой начинается зона пула слов
    qint32 m_poolStarts;
};

#endif // CAREAMANAGER_H
