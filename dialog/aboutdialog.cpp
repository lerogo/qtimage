#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include<QProcess>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    //设置图片
    QPixmap *pixmap = new QPixmap(":/img/SimpleImage.svg");
    pixmap->scaled(ui->label_Pic->size(), Qt::KeepAspectRatio);
    ui->label_Pic->setScaledContents(true);
    ui->label_Pic->setPixmap(*pixmap);
    //设置字
    QFont font ("Microsoft YaHei", 16, 75);
    ui->label_Text->setText("Simple Image");
    ui->label_Text->setFont(font);
    ui->label_Text->setAlignment(Qt::AlignHCenter);
    ui->label_Text->setStyleSheet("color:#12BCFB;");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

//三个button
void AboutDialog::on_pushButton_2_clicked()
{
    QProcess::startDetached("explorer https://www.lerogo.top");
}

void AboutDialog::on_pushButton_clicked()
{
    QProcess::startDetached("explorer https://www.lerogo.top");
}


void AboutDialog::on_pushButton_3_clicked()
{
    QProcess::startDetached("explorer https://www.lerogo.top");
}
