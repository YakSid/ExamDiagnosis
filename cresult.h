#ifndef CRESULT_H
#define CRESULT_H

#include <QDialog>
#include <QMap>

// TODO: СЕГОДНЯ окно вывода результата
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

private:
    Ui::CResult *ui;
};

#endif // CRESULT_H
