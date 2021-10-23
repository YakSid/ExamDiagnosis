#include "cmenu.h"
#include "ui_cmenu.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

const auto STORAGE_FILENAME = QStringLiteral("path.ini");

CMenu::CMenu(QWidget *parent) : QDialog(parent), ui(new Ui::CMenu)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    m_settings = new CMenuSettings(this);

    m_pathToTests = _getPath();
    if (!m_pathToTests.isEmpty()) {
        _fillLw(m_pathToTests);
        m_settings->setPath(m_pathToTests);
    }
}

CMenu::~CMenu()
{
    delete ui;
}

void CMenu::on_pb_settings_clicked()
{
    m_settings->setModal(true);
    m_settings->exec();

    auto path = m_settings->getPath();
    if (!path.isEmpty()) {
        _savePath(path);
        _fillLw(path);
    }
}

void CMenu::on_pb_startTest_clicked()
{
    m_mode = 1;
    close();
}

void CMenu::_fillLw(QString path)
{
    // TODO: СЕЙЧАС открывать правильный тест выбранный в lw, не откр если не выбр
}

void CMenu::_savePath(QString path)
{
    QFile storage(STORAGE_FILENAME);
    storage.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream writeStream(&storage);
    writeStream << path;
    storage.close();
}

QString CMenu::_getPath()
{
    QString path;
    QFile storage(STORAGE_FILENAME);
    storage.open(QIODevice::ReadOnly);
    QTextStream in(&storage);
    while (!in.atEnd()) {
        path = in.readLine();
    }
    storage.close();
    return path;
}
