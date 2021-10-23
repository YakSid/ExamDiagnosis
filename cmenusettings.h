#ifndef CMENUSETTINGS_H
#define CMENUSETTINGS_H

#include <QDialog>

namespace Ui {
class CMenuSettings;
}

class CMenuSettings : public QDialog
{
    Q_OBJECT

public:
    explicit CMenuSettings(QWidget *parent = nullptr);
    ~CMenuSettings();

    QString getPath() { return m_path; }
    void setPath(QString path);

private slots:
    void on_pb_changePath_clicked();
    void on_pb_ok_clicked();

private:
    Ui::CMenuSettings *ui;

    QString m_path;
};

#endif // CMENUSETTINGS_H
