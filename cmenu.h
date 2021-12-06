#ifndef CMENU_H
#define CMENU_H

#include <QDialog>
#include "cmenusettings.h"

namespace Ui {
class CMenu;
}

class CMenu : public QDialog
{
    Q_OBJECT

public:
    explicit CMenu(QWidget *parent = nullptr);
    ~CMenu();

    quint8 getMode() { return m_mode; }
    void updateMode() { m_mode = 0; }
    QString getPathToSelectedTest();

private slots:
    void on_pb_settings_clicked();
    void on_pb_startTest_clicked();

private:
    //! Заполнить lw тестами из указанной папки
    void _fillLw(QString path);
    //Работа с ini
    void _savePath(QString path);
    QString _getPath();

private:
    Ui::CMenu *ui;
    CMenuSettings *m_settings { nullptr };

    quint8 m_mode { 0 };
    QString m_pathToTests;
    //! Путь к выбранному тесту
    QString m_selectedPath;
};

#endif // CMENU_H
