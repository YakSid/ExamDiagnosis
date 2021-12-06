#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "careamanager.h"
#include "cmenu.h"
#include "cjsonmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_checkAnswer_clicked();
    void on_pb_back_clicked();

private:
    //! Ищем нужные слова - фикс бага программы Writer
    QList<SWord *> _findWordsFixWritersBug();

private:
    Ui::MainWindow *ui;
    CAreaManager *m_areaMg { nullptr };
    CMenu *m_menu { nullptr };

    SExample m_example;
};

#endif // MAINWINDOW_H
