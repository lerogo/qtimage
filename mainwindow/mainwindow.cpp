#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsPixmapItem>

//初始化
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化
    leftScene = new QGraphicsScene;//类 用于在场景中操作大量的2D图形元素
    leftPixmapItem = new QGraphicsPixmapItem();//存图片

    rightScene = new QGraphicsScene;
    rightPixmapItem = new QGraphicsPixmapItem();

    //初始化
    Pixel = new QLabel;
    size = new QLabel;
    info = nullptr;
    ConstImage="";
    flag=false;//用于退出确认

    //初始化界面m
    leftScene->setBackgroundBrush(QColor::fromRgb(224,224,224));
    ui->leftGraphicsView->setScene(leftScene);
    rightScene->setBackgroundBrush(QColor::fromRgb(224,224,224));
    ui->rightGraphicsView->setScene(rightScene);

    //初始化底部statusBar
    ui->statusBar->addPermanentWidget(size);
    ui->statusBar->addPermanentWidget(Pixel);

    createToolBar();
    setActionStatus(false);
    setWindowTitle("Simple Image");

}

//添加bar
void MainWindow::createToolBar()
{
    ui->toolBar_File->addAction(ui->doOpen);
    ui->toolBar_File->addAction(ui->doSave);
    ui->toolBar_File->addAction(ui->doSave_As);
    ui->toolBar_File->addSeparator();
    ui->toolBar_File->addAction(ui->doClose);
    ui->toolBar_File->addAction(ui->doRestore);

    ui->toolBar_Edit->addAction(ui->do_adjust);
    ui->toolBar_Edit->addAction(ui->do_adjust_left);
    ui->toolBar_Edit->addAction(ui->do_adjust_right);
    ui->toolBar_Edit->addSeparator();
    ui->toolBar_Edit->addAction(ui->doRotate);
    ui->toolBar_Edit->addAction(ui->doRotate_left);
    ui->toolBar_Edit->addAction(ui->doRotate_right);
    ui->toolBar_Edit->addSeparator();
    ui->toolBar_Edit->addAction(ui->doHorizontal);
    ui->toolBar_Edit->addAction(ui->doVertical);

    ui->toolBar_Change->addAction(ui->doZoom);
    ui->toolBar_Change->addAction(ui->doBrightness);
    ui->toolBar_Change->addSeparator();
    ui->toolBar_Change->addAction(ui->doWarm);
    ui->toolBar_Change->addAction(ui->doCool);
    ui->toolBar_Change->addSeparator();
    ui->toolBar_Change->addAction(ui->doMovie_frame);
    ui->toolBar_Change->addAction(ui->doClassic_frame);
    ui->toolBar_Change->addAction(ui->doFlower_frame);

    ui->toolBar_Special->addAction(ui->doGrayscale);
    ui->toolBar_Special->addAction(ui->doContour_extraction);
    ui->toolBar_Special->addSeparator();
    ui->toolBar_Special->addAction(ui->doSimple);
    ui->toolBar_Special->addAction(ui->doGauss);
    ui->toolBar_Special->addSeparator();
    ui->toolBar_Special->addAction(ui->doMeida_Filter);
    ui->toolBar_Special->addAction(ui->doLaplace);

    ui->toolBar_Cal->addAction(ui->doBinaryzation);
    ui->toolBar_Cal->addSeparator();
    ui->toolBar_Cal->addAction(ui->doDilate);
    ui->toolBar_Cal->addAction(ui->doExpansion);
    ui->toolBar_Cal->addSeparator();
    ui->toolBar_Cal->addAction(ui->doOpening);
    ui->toolBar_Cal->addAction(ui->doClosing);

    ui->toolBar_About->addAction(ui->doAbout);
    ui->toolBar_About->addSeparator();
    ui->toolBar_About->addAction(ui->doExit);
}


//关闭窗口时
MainWindow::~MainWindow()
{


    delete ui;

    if (leftScene)
    {
        delete leftScene;
        leftScene = nullptr;
    }

    if (size)
    {
        delete size;
        size = nullptr;
    }

    if (rightScene)
    {
        delete rightScene;
        rightScene = nullptr;
    }
}
void MainWindow::closeEvent(QCloseEvent *event){
    //是否更改
    if(flag){
        if(QMessageBox::Yes==QMessageBox::question(this,tr("尚未保存"),tr("是否保存？"),QMessageBox::Yes | QMessageBox::No,QMessageBox::No)){
            on_doSave_triggered();
        }
    }
    if(QMessageBox::Yes==QMessageBox::question(this,tr("退出"),tr("是否退出？"),QMessageBox::Yes | QMessageBox::No,QMessageBox::No)){
        event->accept();
    }else {
        event->ignore();
    }

}

//未加载图片，将图片处理按钮设为不可用
void MainWindow::setActionStatus(bool status)
{
    ui->doCool->setEnabled(status);
    ui->doWarm->setEnabled(status);
    ui->doGauss->setEnabled(status);
    ui->doDilate->setEnabled(status);
    ui->doRotate->setEnabled(status);
    ui->doSimple->setEnabled(status);
    ui->doClosing->setEnabled(status);
    ui->doLaplace->setEnabled(status);
    ui->doOpening->setEnabled(status);
    ui->doRestore->setEnabled(status);
    ui->doSave->setEnabled(status);
    ui->doSave_As->setEnabled(status);
    ui->do_adjust->setEnabled(status);
    ui->doVertical->setEnabled(status);
    ui->doExpansion->setEnabled(status);
    ui->doGrayscale->setEnabled(status);
    ui->doHorizontal->setEnabled(status);
    ui->doMovie_frame->setEnabled(status);
    ui->doBinaryzation->setEnabled(status);
    ui->doFlower_frame->setEnabled(status);
    ui->doMeida_Filter->setEnabled(status);
    ui->do_adjust_left->setEnabled(status);
    ui->doClassic_frame->setEnabled(status);
    ui->do_adjust_right->setEnabled(status);
    ui->doBrightness->setEnabled(status);
    ui->doContour_extraction->setEnabled(status);
    ui->doZoom->setEnabled(status);
    ui->doClose->setEnabled(status);
    ui->doRotate_left->setEnabled(status);
    ui->doRotate_right->setEnabled(status);
}


/**********************************************************************************
 *                                    打开
 * ********************************************************************************/
void MainWindow::on_doOpen_triggered()
{
    if(flag){
        if(QMessageBox::Yes==QMessageBox::question(this,tr("尚未保存"),tr("是否返回操作？"),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes)){
            return;
        }
    }
    /*
     *TODO:第一次打开桌面选择,第二次打开上次目录
     */
    if(ConstImage==""){
        ConstImage=getUserPath();
    }
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Open image"), ConstImage,
                                                     tr("All Images (*.bpm *.jpg *.jpeg *.png *.tif *.pcx *.tga *.svg *.ico);;"
                                                        "All Files (*);;"
                                                        "Image BPM (*.bpm);;"
                                                        "Image JPG (*.jpg);;"
                                                        "Image JPEG (*.jpeg);;"
                                                        "Image PNG (*.png);;"));
    ConstImage=imagePath;
    //如果这个目录存在
    if (!imagePath.isEmpty())//首先地址
    {
        QFile file(imagePath);
        //指定打开文件模式
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr(WINDOW_ERROR),
                                  tr("不能打开这个文件！"));
            return;
        }

        //将之前的图片清除掉
        cleanImage();

        // 更新图片信息
        info = new QFileInfo(imagePath);

        //左框显示图片
        QPixmap leftPixmap(imagePath);//图片地址
        leftPixmapItem = leftScene->addPixmap(leftPixmap);//设置图片
        leftScene->setSceneRect(QRectF(leftPixmap.rect()));//更新坐标
        //右框显示图片
        QPixmap rightPixmap(imagePath);
        rightPixmapItem = rightScene->addPixmap(rightPixmap);
        rightScene->setSceneRect(QRectF(rightPixmap.rect()));

        //调整使得图片适应边框
        on_do_adjust_triggered();

        //更改软件标题
        this->setWindowTitle(info->fileName() + " - SImple Image");

        //设置操作可用
        setActionStatus(true);

        //statusBar更新信息
        double size_h=info->size()*1.0/1024;
        QString size_type=" KB";
        if(size_h>=1024){
            size_h/=1024;
            size_type=" MB";
        }
        size->setText(QString::number(size_h, 'f', 2)+size_type);
        Pixel->setText(QString::number(leftPixmapItem->pixmap().width())
                       + " x " + QString::number(leftPixmapItem->pixmap().height()));
        flag=false;
    }

}

/**********************************************************************************
 *                                    关闭
 * ********************************************************************************/
void MainWindow::on_doClose_triggered()
{
    if(flag){
        if(QMessageBox::Yes==QMessageBox::question(this,tr("尚未保存"),tr("是否返回操作再关闭？"),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes)){
            return;
        }
    }
    //直接清理图片
    cleanImage();
    flag=false;
}

/**********************************************************************************
 *                                  清理图片
 * ********************************************************************************/
void MainWindow::cleanImage()
{
    //左边框清理
    leftScene->clear();
    //重置
    ui->leftGraphicsView->resetTransform();
    //右边
    rightScene->clear();
    ui->rightGraphicsView->resetTransform();

    //更新statusBar
    if (size)
    {
        delete size;
        size = new QLabel;
        ui->statusBar->addPermanentWidget(size);
    }
    if(Pixel){
        delete Pixel;
        Pixel = new QLabel;
        ui->statusBar->addPermanentWidget(Pixel);
    }
    //还原标题
    this->setWindowTitle(WINDOW_TITLE);

    //控件设为不可用
    setActionStatus(false);
}


/**********************************************************************************
 *                                     保存
 * ********************************************************************************/
void MainWindow::on_doSave_triggered()
{
    //首先将右边图与本地图片进行替换
    rightPixmapItem->pixmap().save(ConstImage);

    //清楚左边框图片
    leftScene->clear();
    ui->leftGraphicsView->resetTransform();

    //更新底部statusBar
    if (size)
    {
        delete size;
        size = new QLabel;
        ui->statusBar->addPermanentWidget(size);
    }
    if(Pixel){
        delete Pixel;
        Pixel = new QLabel;
        ui->statusBar->addPermanentWidget(Pixel);
    }

    //重新获取图片信息
    info = new QFileInfo(ConstImage);

    //左框重新显示图片
    QPixmap leftPixmap(ConstImage);
    leftPixmapItem = leftScene->addPixmap(leftPixmap);
    leftScene->setSceneRect(QRectF(leftPixmap.rect()));

    //更新底部statusBar
    double size_h=info->size()*1.0/1024;
    QString size_type=" KB";
    if(size_h>=1024){
        size_h/=1024;
        size_type=" MB";
    }
    size->setText(QString::number(size_h, 'f', 2)+size_type);
    Pixel->setText(QString::number(leftPixmapItem->pixmap().width())
                   + " x " + QString::number(leftPixmapItem->pixmap().height()));

    //仅调整左边图片适应窗口
    on_do_adjust_left_triggered();
    flag=false;
}

/**********************************************************************************
 *                                    另存为
 * ********************************************************************************/
void MainWindow::on_doSave_As_triggered()
{

    //直接用原始图片作为地址和名字
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save image"), ConstImage,
                                                   tr("All files (*);;"
                                                      "Image BPM (*.bpm);;"
                                                      "Image JPG (*.jpg);;"
                                                      "Image JPEG (*.jpeg);;"
                                                      "Image PNG (*.png);;"
                                                      "Image SVG (*.svg);;"
                                                      "Image ICO (*.ico);;"
                                                      "Image TIF (*.tif);;"
                                                      "Image PCX (*.pcx);;"
                                                      "Image TGA (*.tga);;"));

    //检测是否能够保存 逻辑同打开
    if (!newPath.isEmpty()) {
        QFile file(newPath);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, tr(WINDOW_ERROR), tr("不能保存这个文件！"));
            return;
        }
        rightPixmapItem->pixmap().save(newPath);
        flag=false;
    }
}

/**********************************************************************************
 *                                    退出
 * ********************************************************************************/
void MainWindow::on_doExit_triggered()
{
    //检查是否更改
    if(flag){
        if(QMessageBox::Yes==QMessageBox::question(this,tr("尚未保存"),tr("是否返回操作再退出？"),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes)){
            return;
        }
    }
    cleanImage();
    qApp->quit();
}


/**********************************************************************************
 *                                    更新右边图片
 * ********************************************************************************/
void MainWindow::updateRightImage(QPixmap &pixmap)
{
    //刷新右边图片
    rightPixmapItem->setPixmap(pixmap);
    rightScene->setSceneRect(QRectF(pixmap.rect()));
}

/**********************************************************************************
 *                                 槽函数恢复右边
 * ********************************************************************************/
void MainWindow::on_doRestore_triggered()
{
    //获取左侧图片
    QPixmap leftImage = leftPixmapItem->pixmap();

    //用左侧图片更新右侧
    updateRightImage(leftImage);
    //调整图像自适应
    on_do_adjust_right_triggered();
    flag=false;
}


/**********************************************************************************
 *                                    灰度
 * ********************************************************************************/
void MainWindow::on_doGrayscale_triggered()
{
    //首先获取原先的rightImage
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 灰度图片 调用Algorithm::GreyScale()
    QImage newImage = Algorithm::GreyScale(rightImage.toImage());//Algorithm.cpp

    //将QImage转为QPixmap
    rightImage.convertFromImage(newImage);

    //更新图片
    updateRightImage(rightImage);
    flag=true;
}



/**********************************************************************************
 *                                    电影边框
 * ********************************************************************************/
void MainWindow::on_doMovie_frame_triggered()
{
    //首先获取原先的rightImage
    QPixmap rightImage = rightPixmapItem->pixmap();

    //加载边框资源
    QImage frame = QImage(":/img/frame_3.png");

    //效果处理 添加边框 调用Algorithm::DrawFrame()
    QImage newImage = Algorithm::DrawFrame(rightImage.toImage(), frame);

    //将QImage转为QPixmap
    rightImage.convertFromImage(newImage);

    //更新图片
    updateRightImage(rightImage);
    flag=true;
}
/**********************************************************************************
 *                                    经典边框
 * ********************************************************************************/
void MainWindow::on_doClassic_frame_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();
    QImage frame = QImage(":/img/frame_1.png");

    //效果处理 添加边框 调用Algorithm::DrawFrame()
    QImage newImage = Algorithm::DrawFrame(rightImage.toImage(), frame);

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}
/**********************************************************************************
 *                                    小花边框
 * ********************************************************************************/
void MainWindow::on_doFlower_frame_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();
    QImage frame = QImage(":/img/frame_2.png");

    //效果处理 添加边框 调用Algorithm::DrawFrame()
    QImage newImage = Algorithm::DrawFrame(rightImage.toImage(), frame);

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}


/**********************************************************************************
 *                                    冷
 * ********************************************************************************/
void MainWindow::on_doCool_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 色素变幻 调用Algorithm::Cool()
    QImage newImage = Algorithm::Cool(30, rightImage.toImage());

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}

/**********************************************************************************
 *                                    温
 * ********************************************************************************/
void MainWindow::on_doWarm_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 色素变幻 调用Algorithm::Warm()
    QImage newImage = Algorithm::Warm(30, rightImage.toImage());

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}



/**********************************************************************************
 *                                    简单平滑
 * ********************************************************************************/
void MainWindow::on_doSimple_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 像素点处理 调用Algorithm::SimpleSmooth()
    QImage newImage = Algorithm::SimpleSmooth(rightImage.toImage());

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}
/**********************************************************************************
 *                                    高斯
 * ********************************************************************************/
void MainWindow::on_doGauss_triggered()
{
    //输入框
    GaussianBlurDialog dialog;
    //信号处理
    connect(&dialog, SIGNAL(sendData(int, double)), this,
            SLOT(receiveGaussValue(int, double)));
    //阻塞对话框
    dialog.exec();
}
//高斯处理
void MainWindow::receiveGaussValue(int radius, double sigma)
{
    //new对象
    GaussBlur *blur = new GaussBlur(radius, sigma);

    QPixmap rightImage = rightPixmapItem->pixmap();

    //QPixmap转化为image之后进调用BlurImage()进行处理
    QImage newImage = blur->StartGauss(rightImage.toImage());

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}

/**********************************************************************************
 *                                    中值虑波
 * ********************************************************************************/
void MainWindow::on_doMeida_Filter_triggered()
{
    //快速创建对话框
    bool isOK;
    int value = QInputDialog::getInt(this, tr("中值滤波"), "请输入一个参数(1-20)",3,1,20,1,&isOK);
    if (isOK)
    {
        QPixmap rightImage = rightPixmapItem->pixmap();

        //效果处理 中值滤波 调用Algorithm::MeidaFilter()
        QImage newImage = Algorithm::MeidaFilter(rightImage.toImage(), value);

        rightImage.convertFromImage(newImage);
        updateRightImage(rightImage);
        flag=true;
    }
}


/**********************************************************************************
 *                                    缩放
 * ********************************************************************************/
void MainWindow::on_doZoom_triggered()
{
    //快速创建对话框
    bool isOK;
    int LocalValue = QInputDialog::getInt(this, tr("缩放"), "请输入缩放值(10-1000)(%)",100,10,1000,10,&isOK);
    if (isOK)
    {
        if (LocalValue != 100)
        {
            QPixmap rightImage = rightPixmapItem->pixmap();

            int cur_width = rightImage.width();
            int cur_height = rightImage.height();

            //效果处理 缩放 调用自带缩放
            QPixmap newPixmap = rightImage.scaled(cur_width*LocalValue/100, cur_height*LocalValue/100);

            updateRightImage(newPixmap);
            flag=true;
        }
        else
        {
            return;
        }
    }
}

/**********************************************************************************
 *                                    水平翻转
 * ********************************************************************************/
void MainWindow::on_doHorizontal_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 水平翻转 调用Algorithm::Horizontal()
    QImage newImage = Algorithm::Horizontal(rightImage.toImage());

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}

/**********************************************************************************
 *                                    垂直翻转
 * ********************************************************************************/
void MainWindow::on_doVertical_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 垂直翻转 调用Algorithm::Vertical()
    QImage newImage = Algorithm::Vertical(rightImage.toImage());

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}

/**********************************************************************************
 *                                    锐化
 * ********************************************************************************/
void MainWindow::on_doLaplace_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 锐化 调用Algorithm::LaplaceSharpen()
    QImage newImage = Algorithm::LaplaceSharpen(rightImage.toImage());

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}


/**********************************************************************************
 *                                    二值化
 * ********************************************************************************/
void MainWindow::on_doBinaryzation_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 二值化 调用Algorithm::Binaryzation()
    QImage newImage = Algorithm::Binaryzation(rightImage.toImage());

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}

/**********************************************************************************
 *                                    亮度
 * ********************************************************************************/
void MainWindow::on_doBrightness_triggered()
{
    bool isOK;
    int delta = QInputDialog::getInt(this,tr("亮度"),"请输入参数(+/-)",0,-1000,1000,10,&isOK);
    if (isOK)
    {
        if (delta != 100)
        {
            QPixmap rightImage = rightPixmapItem->pixmap();

            //效果处理 调整亮度 调用Algorithm::Brightness()
            QImage newImage = Algorithm::Brightness(delta, rightImage.toImage());

            rightImage.convertFromImage(newImage);
            updateRightImage(rightImage);
            flag=true;
        }
        else
        {
            return;
        }
    }

}


/**********************************************************************************
 *                                    关于
 * ********************************************************************************/
void MainWindow::on_doAbout_triggered()
{
    AboutDialog dialog;
    dialog.exec();
}


/******************************************************************************
 *                              获得当前用户的桌面目录
 *****************************************************************************/
QString MainWindow::getUserPath()
{
    QString userPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    return userPath;
}

/**********************************************************************************
 *                                    轮廓提取
 * ********************************************************************************/
void MainWindow::on_doContour_extraction_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 轮廓提取 调用Algorithm::ContourExtraction()
    QImage newImage = Algorithm::ContourExtraction(rightImage.toImage());

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}


/**********************************************************************************
 *                                    腐蚀
 * ********************************************************************************/
void MainWindow::on_doDilate_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 腐蚀 调用Algorithm::Binaryzation() & Algorithm::Dilate()
    QImage binaryImage = Algorithm::Binaryzation(rightImage.toImage());
    QImage newImage = Algorithm::Dilate(binaryImage);

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}

/**********************************************************************************
 *                                    膨胀
 * ********************************************************************************/
void MainWindow::on_doExpansion_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 膨胀 调用Algorithm::Binaryzation() & Algorithm::Expansion()
    QImage binaryImage = Algorithm::Binaryzation(rightImage.toImage());
    QImage newImage = Algorithm::Expansion(binaryImage);

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}
/**********************************************************************************
 *                                    开运算
 * ********************************************************************************/
void MainWindow::on_doOpening_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 开运算 调用Algorithm::Binaryzation() & Algorithm::Opening()
    QImage binaryImage = Algorithm::Binaryzation(rightImage.toImage());
    QImage newImage = Algorithm::Opening(binaryImage);

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}

/**********************************************************************************
 *                                    闭运算
 * ********************************************************************************/
void MainWindow::on_doClosing_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();

    //效果处理 闭运算 调用Algorithm::Binaryzation() & Algorithm::Closing()
    QImage binaryImage = Algorithm::Binaryzation(rightImage.toImage());
    QImage newImage = Algorithm::Closing(binaryImage);

    rightImage.convertFromImage(newImage);
    updateRightImage(rightImage);
    flag=true;
}

/**********************************************************************************
 *                                    旋转
 * ********************************************************************************/
void MainWindow::on_doRotate_triggered()
{
    bool isOK;
    int LocalValue = QInputDialog::getInt(this, tr("旋转"), "请输入要旋转的角度（正数向右，负数向左）",0,-360,360,10,&isOK);
    if (isOK)
    {
        if (LocalValue != 0)
        {
            QPixmap rightImage = rightPixmapItem->pixmap();
            QImage *imgRotate = new QImage;
            QMatrix matrix;
            matrix.rotate(LocalValue);
            *imgRotate = rightImage.toImage().transformed(matrix);
            QPixmap newPixmap;
            newPixmap = QPixmap::fromImage(*imgRotate);
            updateRightImage(newPixmap);
            flag=true;
        }
        else
        {
            return;
        }
    }
}

void MainWindow::on_doRotate_left_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();
    QImage *imgRotate = new QImage;
    QMatrix matrix;
    matrix.rotate(-90);
    *imgRotate = rightImage.toImage().transformed(matrix);
    QPixmap newPixmap;
    newPixmap = QPixmap::fromImage(*imgRotate);
    updateRightImage(newPixmap);
    flag=true;
}

void MainWindow::on_doRotate_right_triggered()
{
    QPixmap rightImage = rightPixmapItem->pixmap();
    QImage *imgRotate = new QImage;
    QMatrix matrix;
    matrix.rotate(90);
    *imgRotate = rightImage.toImage().transformed(matrix);
    QPixmap newPixmap;
    newPixmap = QPixmap::fromImage(*imgRotate);
    updateRightImage(newPixmap);
    flag=true;
}

/**********************************************************************************
 *                                    调整图片适应界面
 * ********************************************************************************/
void MainWindow::on_do_adjust_triggered()
{
    on_do_adjust_left_triggered();
    on_do_adjust_right_triggered();
}

/*****************************************************************************
 *                            调整左边图片适应
 * **************************************************************************/
void MainWindow::on_do_adjust_left_triggered()
{
    //恢复
    ui->leftGraphicsView->resetTransform();

    //左图
    int height = leftPixmapItem->pixmap().height();
    int width = leftPixmapItem->pixmap().width();
    int max_height = ui->leftGraphicsView->height();
    int max_width = ui->leftGraphicsView->width();

    //计算
    int pic_size = qMax(width,height);
    int max_size = qMin(max_width,max_height)-5;
    double val = (max_size*1.0)/pic_size;

    ui->leftGraphicsView->scale(val,val);
}
/*****************************************************************************
 *                            调整右边图片适应
 * **************************************************************************/
void MainWindow::on_do_adjust_right_triggered()
{
    //恢复
    ui->rightGraphicsView->resetTransform();

    // 右图
    int height = rightPixmapItem->pixmap().height();
    int width = rightPixmapItem->pixmap().width();
    int max_height = ui->rightGraphicsView->height();
    int max_width = ui->rightGraphicsView->width();

    //计算
    int pic_size = qMin(width,height);
    int max_size = qMin(max_width,max_height)-5;

    double val = (max_size*1.0)/pic_size;

    //更新
    ui->rightGraphicsView->scale(val,val);
}
