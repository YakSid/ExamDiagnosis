#ifndef CRESULT_H
#define CRESULT_H

#include <QDialog>
#include <QMap>

#include "cstructs.h"

/// Класс выводит на экран окно с правильными вариантами диагноза

namespace Ui {
class CResult;
}

class CResult : public QDialog
{
    Q_OBJECT

public:
    explicit CResult(QWidget *parent = nullptr);
    ~CResult();

    void init(const SExample &example);

private:
    QString _findWordById(qint32 id);

private:
    Ui::CResult *ui;

    SExample m_example;
};

#endif // CRESULT_H
