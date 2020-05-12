#include "gaussblur.h"
#include <math.h>

/*
 * 算法参考:
 * http://www.ruanyifeng.com/blog/2012/11/gaussian_blur.html
 */

GaussBlur::GaussBlur(int r, double s):Radius(r),Sigma(s)
{
    gs_matrix=nullptr;
    GetMatrix();
}
GaussBlur::~GaussBlur()
{
}
//用于计算权重
double GaussBlur::GaussFormula(double x)
{
    // 一维高斯公式
    //1/σ√(2PI)*exp((-x^2)/2σ^2)
    return (1 / sqrt(2*(M_PI)*pow(Sigma,2))) *exp(-pow(x,2)/(2*pow(Sigma,2)));
}
//返回像素位置，避免数组下标越界
int GaussBlur::GetIndex(int x, int length)
{
    if (x < 0)
        return -x - 1;
    else if(x >= length)
        return 2*length - x - 1;
    return x;
}

void GaussBlur::GetMatrix()
{
    //矩阵大小
    this->matrixSize =((2*Radius + 1));
    this->halfMatrixSize = matrixSize / 2;
    int x = -halfMatrixSize;
    //建立数组
    gs_matrix=new float [unsigned(matrixSize)];
    //存放sum
    float sum=0.0;
    //对每一个点计算权重
    for(int i=0;i<matrixSize;++i){
        gs_matrix[i]=static_cast<float>(GaussFormula(x++));
        sum+=gs_matrix[i];
    }
    //除以sum得到最后的矩阵
    for(int i=0;i<matrixSize;++i){
        gs_matrix[i]/=sum;
    }
}

//程序入口
QImage GaussBlur::StartGauss(const QImage& oldImage)
{
    //用原来图像info初始化图像
    QImage newImage(oldImage.size(), oldImage.format());
    //三色
    float sumRed = 0.0f;
    float sumBlue = 0.0f;
    float sumGreen = 0.0f;
    //暂存像素点
    int _x = 0, _y = 0;

    //遍历每个点
    for (int i = 0; i < newImage.width(); ++i)
    {
        for (int j = 0; j < newImage.height(); ++j)
        {
            /***************************对x方向“宽方向”***************************/
            sumRed = sumGreen = sumBlue = 0.0f;
            //对每个点在“宽”方向进行计算
            for (int k = -halfMatrixSize; k <= halfMatrixSize; ++k)
            {
                //将 "点" 左右两边的取出来与 矩阵做*计算
                //取出需要的点横坐标
                _x = GetIndex(i + k, newImage.width());
                //取得这个像素点信息
                QColor color(oldImage.pixel(_x, j));
                //计算
                sumRed += color.red() * gs_matrix[unsigned(k + halfMatrixSize)];
                sumBlue += color.blue() * gs_matrix[unsigned(k + halfMatrixSize)];
                sumGreen += color.green() * gs_matrix[unsigned(k + halfMatrixSize)];
            }
            //赋值
            QRgb finalColor = qRgb(static_cast<int>(sumRed), static_cast<int>(sumGreen), static_cast<int>(sumBlue));
            newImage.setPixel(i, j, finalColor);

            /***************************对y方向“长方向”***************************/
            sumRed = sumGreen = sumBlue = 0.0f;
            for (int k = -halfMatrixSize; k <= halfMatrixSize; ++k)
            {
                _y = GetIndex(j + k, newImage.height());
                QColor color(oldImage.pixel(i, _y));
                sumRed += color.red() * gs_matrix[unsigned(k + halfMatrixSize)];
                sumBlue += color.blue() * gs_matrix[unsigned(k + halfMatrixSize)];
                sumGreen += color.green() * gs_matrix[unsigned(k + halfMatrixSize)];
            }
            finalColor = qRgb(static_cast<int>(sumRed), static_cast<int>(sumGreen), static_cast<int>(sumBlue));
            newImage.setPixel(i, j, finalColor);
        }
    }

    return newImage;
}



