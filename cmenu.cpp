#include "cmenu.h"
#include "ui_cmenu.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

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

QString CMenu::getPathToSelectedTest()
{
    return m_selectedPath;
}

void CMenu::on_pb_settings_clicked()
{
    m_settings->setModal(true);
    m_settings->exec();

    auto m_pathToTests = m_settings->getPath();
    if (!m_pathToTests.isEmpty()) {
        _savePath(m_pathToTests);
        _fillLw(m_pathToTests);
    }
}

void CMenu::on_pb_startTest_clicked()
{
    if (ui->lw_tests->selectedItems().isEmpty()) {
        QMessageBox msg;
        msg.setText("Выберите тест");
        msg.setStyleSheet("QLabel{min-width: 100px; font-size: 12px;}");
        msg.exec();
        return;
    }

    m_selectedPath = m_pathToTests + "/" + ui->lw_tests->selectedItems().first()->text() + ".json";
    m_mode = 1;
    close();
}

void CMenu::_fillLw(QString path)
{
    ui->lw_tests->clear();

    const QDir source(path);
    if (!source.exists())
        return;
    const QStringList files = source.entryList(QStringList() << "*.json", QDir::Files);

    for (auto name : files) {
        name.chop(5);
        ui->lw_tests->addItem(name);
    }
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
