#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include <QObject>
#include <QtConcurrent>
#include <QProgressDialog>

#define MAX_FILTER_RADIUS 30

//用于建立一系列的点
struct IndexPair{
    int i, j;
    IndexPair(int a, int b)
    {
        i = a; j = b;
    }
};

class MedianFilter : public QObject
{
    Q_OBJECT
public:

    static MedianFilter *StaticThis;//this指针

    explicit MedianFilter(QObject *parent = nullptr);
    ~MedianFilter();

    //apply
    void applyMedianFilter(int *old, int *res, const int imageHeight, const int imageWidth, const int r);
    //像素点值
    void getResPixelValue(IndexPair &p);
public slots:
    void cancelMedianFilter();
private:
    //image原图  workingImage扩充图   desImage返回图
    int *oldimage, *workingImage, *desImage;

    //原图宽度 高度   中值滤波参数    扩充宽度 高度     处理区域
    int oldWidth,oldHeight, radius,WorkWidth,WorkHeight,area;
    //监控窗口
    QFutureWatcher<void> *futureWatcher;
    QProgressDialog *progressDialog;
};

#endif // MEDIANFILTER_H
