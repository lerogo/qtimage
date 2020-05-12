#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>
#include <QDebug>
#include <QStandardPaths>
#include <QTranslator>
#include "graphicsview.h"
#include "../dialog/dialog_gaussblur.h"
#include "../dialog/aboutdialog.h"
#include "../tools/algorithm.h"
#include "../tools/gaussblur.h"
#include "../tools/medianfilter.h"
#define WINDOW_TITLE    "Simple Image"
#define WINDOW_ERROR "Error - Simple Image"
#define WINDOW_WARNING  "Notice - Simple Image"
#define WINDOW_ABOUT    "About - Simple Image"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateRightImage(QPixmap &pixmap);
    void cleanImage();
    void setActionStatus(bool);
    void createToolBar();
    void closeEvent(QCloseEvent *);

private slots:

    // 从子对话框中接收数据
    void receiveGaussValue(int radius, double sigma);

    void on_doOpen_triggered();//打开文件
    void on_doClose_triggered(); //关闭图片
    void on_doSave_triggered();//保存
    void on_doSave_As_triggered();//另存为
    void on_doExit_triggered(); //退出
    void on_doGrayscale_triggered(); //灰度图像
    void on_doBrightness_triggered();//调整亮度
    void on_doRestore_triggered();//恢复右边图像
    void on_doMovie_frame_triggered();//电影边框
    void on_doClassic_frame_triggered();//经典边框
    void on_doFlower_frame_triggered();//花边框
    void on_doCool_triggered();//冷
    void on_doWarm_triggered();//温
    void on_doSimple_triggered();//简单平滑
    void on_doGauss_triggered();//高斯平滑
    void on_doMeida_Filter_triggered();//中值滤波
    void on_doZoom_triggered();         // 缩放
    void on_doHorizontal_triggered();//水平翻转
    void on_doVertical_triggered();//垂直翻转
    void on_doAbout_triggered(); // 关于
    void on_doLaplace_triggered();  //锐化
    void on_doBinaryzation_triggered();//二值化
    void on_doContour_extraction_triggered();//轮廓提取
    void on_doDilate_triggered();//腐蚀
    void on_doExpansion_triggered();//膨胀
    void on_doOpening_triggered();//开运算
    void on_doClosing_triggered();//闭运算
    void on_do_adjust_triggered();// 调整图片适应窗口
    void on_do_adjust_left_triggered();// 调整图片适应左窗口
    void on_do_adjust_right_triggered();// 调整图片适应右窗口
    void on_doRotate_triggered();//旋转
    void on_doRotate_left_triggered();//逆时针旋转90度
    void on_doRotate_right_triggered();//顺时针旋转90度

private:

    Ui::MainWindow  *ui;
    //左右两个图片
    QGraphicsScene  *leftScene;
    QGraphicsScene  *rightScene;
    QGraphicsPixmapItem* leftPixmapItem;
    QGraphicsPixmapItem* rightPixmapItem;

    //图片大小label
    QLabel *size;
    //图片像素
    QLabel *Pixel;
    //文件信息读取
    QFileInfo *info;
    //用于存放图片地址（下次打开，保存，另存）
    QString ConstImage;
    //是否修改图片
    bool flag;
    //用于获取桌面位置
    QString getUserPath();
};

#endif // MAINWINDOW_H
