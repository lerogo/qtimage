#include "medianfilter.h"
#include <QDebug>
#include<iostream>
/*
 *https://blog.csdn.net/liang19890820/article/details/52935118
 */

MedianFilter* MedianFilter::StaticThis = nullptr;
//用于信号和槽的处理
MedianFilter::MedianFilter(QObject *parent) : QObject(parent)
{
    StaticThis = this;
    oldimage = nullptr;
    workingImage = nullptr;
    desImage = nullptr;

    futureWatcher = new QFutureWatcher<void>(this);
    progressDialog = new QProgressDialog(nullptr);


    //sender signal receiver method
    connect(futureWatcher, SIGNAL(progressValueChanged(int)), progressDialog, SLOT(setValue(int)));
    connect(futureWatcher, SIGNAL(progressRangeChanged(int,int)), progressDialog, SLOT(setRange(int,int)));

    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelMedianFilter()));
}
//析构函数
MedianFilter::~MedianFilter()
{
    if (futureWatcher->isRunning())
    {
        futureWatcher->cancel();
        futureWatcher->waitForFinished();
    }

    if(workingImage)
        delete [] workingImage;
}
//futureWatcher取消函数
void MedianFilter::cancelMedianFilter()
{
    if (futureWatcher->isRunning())
    {
        futureWatcher->cancel();
        futureWatcher->waitForFinished();
    }
}

void getResPixelValueST(IndexPair &p)
{
    MedianFilter::StaticThis->getResPixelValue(p);
}

void MedianFilter::getResPixelValue(IndexPair &p)
{
    int k = 0;
    int *window=new int[unsigned(area)];
    for (int i = p.i - radius; i < p.i + radius+1; ++i)
    {
        for (int j = p.j - radius; j < p.j + radius + 1; ++j)
            window[k++] = this->workingImage[i * (WorkWidth) + j];
    }

    //这里直接使用iostream的sort
    //也可以自己写选择排序
    std::sort(window,window+area);

    desImage[(p.i - radius) * oldWidth + (p.j - radius)] = window[area/2];
}

//原图像 目标图像 高度 宽度 半径
void MedianFilter::applyMedianFilter(int *old, int *des, const int imageHeight, const int imageWidth, const int r)
{
    //pass
    if (futureWatcher->isRunning())
    {
        futureWatcher->cancel();
        futureWatcher->waitForFinished();
    }
    //初始化
    this->oldimage = old;
    this->oldWidth = imageWidth;
    this->oldHeight= imageHeight;
    this->radius = r;
    this->desImage = des;

    this->WorkWidth = oldWidth + 2*radius;
    this->WorkHeight = oldHeight + 2*radius;
    this->area = (2*radius+1) * (2*radius+1);

    workingImage = new int[unsigned(WorkWidth) * unsigned(WorkHeight)];

    // 边缘处理
    //首先拷贝整个图片 然后拷贝左右两侧
    for (int i = 0; i < oldHeight; i++)
    {
        memcpy(workingImage + WorkWidth * (i + radius) + radius, oldimage + oldWidth * i, unsigned(oldWidth) * sizeof(int));
        memcpy(workingImage + WorkWidth * (i + radius), oldimage + (oldWidth * i), unsigned(radius) * sizeof(int));
        memcpy(workingImage + (WorkWidth * (i + radius + 1) - radius), oldimage + oldWidth * (i + 1) - radius, unsigned(radius) * sizeof(int));
    }

    //这里拷贝上下两侧图片
    for(int i = 0; i < radius; i++)
    {
        memcpy(workingImage + i * WorkWidth, workingImage + WorkWidth * (i + radius), unsigned(WorkWidth) * sizeof(int));
        memcpy(workingImage + WorkWidth * (oldHeight + radius + i), workingImage + WorkWidth * (oldHeight + radius - 1 -i), unsigned(WorkWidth) * sizeof(int));
    }

    //将图片每个点存入list 好用QtConcurrent::map实现遍历
    QList<IndexPair> indexes;
    for (int i = radius; i < oldHeight + radius; i++)
    {   for (int j = radius; j < oldWidth + radius; j++)
        {
            IndexPair p(i,j);
            indexes<<p;
        }
    }

    //将每一个像素点执行一次计算
    futureWatcher->setFuture(QtConcurrent::map(indexes, getResPixelValueST));

    progressDialog->exec();
    futureWatcher->waitForFinished();

}
