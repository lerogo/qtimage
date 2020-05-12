#include "dialog_gaussblur.h"
#include "ui_gaussblurdialog.h"

GaussianBlurDialog::GaussianBlurDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GaussianBlurDialog)
{
    ui->setupUi(this);
}

GaussianBlurDialog::~GaussianBlurDialog()
{
    delete ui;
}

//信号处理
void GaussianBlurDialog::on_buttonOK_accepted()
{
    emit sendData(ui->radiusSpinBox->value(), ui->sigmaSpinBox->value());
}

