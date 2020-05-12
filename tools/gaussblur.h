#ifndef GAUSSBLUR_H
#define GAUSSBLUR_H

#include <vector>

#include "qcolor.h"
#include "qimage.h"

using std::vector;

class GaussBlur
{

public:
    GaussBlur(int blurRadius, double sigma);
    ~GaussBlur();
    QImage StartGauss(const QImage& in);

private:
    //用于存放高斯模糊所需的举证
    float *gs_matrix;
    //矩阵大小和一半长
    int matrixSize,halfMatrixSize;
    //两个参数
    int Radius;
    double  Sigma;
    //高斯公式
    double GaussFormula(double x);
    //获取坐标
    int GetIndex(int x, int length);
    //计算矩阵
    void GetMatrix();
};

#endif // GAUSSBLUR_H
