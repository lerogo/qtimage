#ifndef TOOLS_H
#define TOOLS_H

#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QPainter>
#include <QtMath>
#include "gaussblur.h"
//算法部分
namespace  Algorithm {
QImage GreyScale(QImage origin);  //灰度处理
QImage Warm(int delta, QImage origin);//暖色调
QImage Cool(int delta, QImage origin);//冷色调
QImage DrawFrame(QImage origin, QImage &frame);//添加相框
QImage Brightness(int delta, QImage origin);//亮度
QImage Horizontal(const QImage &origin);//水平翻转
QImage Vertical(const QImage &origin);//垂直翻转
QImage SimpleSmooth(const QImage &origin);//简单平滑处理
QImage MeidaFilter(const QImage &origin, int radius);//中值滤波
QImage LaplaceSharpen(const QImage &origin);//拉普拉斯锐化
QImage GaussianSmoothing(const QImage &origin, int radius, double sigma);//高斯平滑
QImage Binaryzation(const QImage &origin);//二值化
QImage ContourExtraction(const QImage &origin);//轮廓提取法
QImage Dilate(const QImage &origin);//全方位腐蚀
QImage Expansion(const QImage &origin);//全方位膨胀
QImage Opening(const QImage &origin);//开运算
QImage Closing(const QImage &origin);//闭运算
}

#endif // TOOLS_H
