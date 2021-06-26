#ifndef CAREAMANAGER_H
#define CAREAMANAGER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

class CAreaManager : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CAreaManager(QWidget *parent = nullptr);
    ~CAreaManager();

signals:

public slots:

private:
    QGraphicsScene *m_scene;
};

#endif // CAREAMANAGER_H
