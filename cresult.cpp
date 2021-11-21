#include "cresult.h"
#include "ui_cresult.h"

CResult::CResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CResult)
{
    ui->setupUi(this);
}

CResult::~CResult()
{
    delete ui;
}
