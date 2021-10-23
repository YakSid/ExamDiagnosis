#include "cmenusettings.h"
#include "ui_cmenusettings.h"

#include <QFileDialog>
#include <QDebug>

CMenuSettings::CMenuSettings(QWidget *parent) : QDialog(parent), ui(new Ui::CMenuSettings)
{
    ui->setupUi(this);
}

CMenuSettings::~CMenuSettings()
{
    delete ui;
}

void CMenuSettings::setPath(QString path)
{
    m_path = path;
    if (ui)
        ui->ln_path->setText(m_path);
}

void CMenuSettings::on_pb_changePath_clicked()
{
    QString path;
    path = QFileDialog::getExistingDirectory(this, tr("Выберите папку с тестами"), "",
                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!path.isEmpty()) {
        m_path = path;
        ui->ln_path->setText(m_path);
    }
}

void CMenuSettings::on_pb_ok_clicked()
{
    close();
}
