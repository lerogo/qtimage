#ifndef GAUSSIANBLURDIALOG_H
#define GAUSSIANBLURDIALOG_H

#include <QDialog>

namespace Ui {
class GaussianBlurDialog;
}

class GaussianBlurDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GaussianBlurDialog(QWidget *parent = nullptr);
    ~GaussianBlurDialog();

private:
    Ui::GaussianBlurDialog *ui;

//信号
signals:
    void sendData(int, double);
//槽
private slots:
    void on_buttonOK_accepted();
};

#endif // GAUSSIANBLURDIALOG_H
