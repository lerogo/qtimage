#include "algorithm.h"
#include "medianfilter.h"
/*
 * 参考文献:
 * https://blog.csdn.net/Aidam_Bo/article/details/84391558
 * https://www.cnblogs.com/kekec/p/3643578.html
 * https://blog.csdn.net/bagboy_taobao_com/article/details/5574159
 * https://blog.csdn.net/chen134225/article/details/80874367
 */

/*****************************************************************************
 *                            灰度处理
 * 计算公式:
 * Gray = 0.299*R+0.587*G+0.114*B
 * 也可以直接使用qGray(origin.pixel(i,j))->使用公式:(r*11+g*16+b*5)/32
 * **************************************************************************/
QImage Algorithm::GreyScale(QImage origin)
{
    //以原图的大小生成一张图片
    QImage *newImage = new QImage(origin.width(), origin.height(),QImage::Format_ARGB32);

    QColor oldColor;

    for (int i=0; i<newImage->width(); ++i) {
        for (int j=0; j<newImage->height(); ++j) {
            //读取原图片的Rgb
            oldColor = QColor(origin.pixel(i,j));
            //计算公式
            int x = (oldColor.red()*299+oldColor.green()*587+oldColor.blue()*114)/1000;
            //设置新图片的Rgb
            newImage->setPixel(i,j,qRgb(x,x,x));
        }
    }
    return *newImage;
}

/*****************************************************************************
 *                              暖色调
 * 公式:
 * 增加黄色=红色R+delta && 绿色G+delta
 * 并保证在[0,255]范围
 * **************************************************************************/
QImage Algorithm::Warm(int delta, QImage origin)
{
    QImage *newImage = new QImage(origin.width(), origin.height(),QImage::Format_ARGB32);

    QColor oldColor;
    int r, g, b;

    for (int i=0; i<newImage->width(); ++i)
    {
        for (int j=0; j<newImage->height(); ++j)
        {
            oldColor = QColor(origin.pixel(i,j));

            //增加黄色
            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue();

            //保证在0到255之间
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);

            newImage->setPixel(i,j, qRgb(r,g,b));
        }
    }
    return *newImage;
}

/*****************************************************************************
 *                           冷色调
 * 同理暖色调，但是更改成相反
 * **************************************************************************/
QImage Algorithm::Cool(int delta, QImage origin)
{
    QImage *newImage = new QImage(origin.width(), origin.height(),
                                  QImage::Format_ARGB32);

    QColor oldColor;
    int r, g, b;

    for (int  i=0; i<newImage->width(); ++i)
    {
        for (int j=0; j<newImage->height(); ++j)
        {
            oldColor = QColor(origin.pixel(i,j));

            r = oldColor.red();
            g = oldColor.green();
            b = oldColor.blue() + delta;

            b = qBound(0, r, 255);

            newImage->setPixel(i,j, qRgb(r,g,b));
        }
    }
    return *newImage;
}



/*****************************************************************************
 *                                亮度
 * 冷热一起调
 * **************************************************************************/
QImage Algorithm::Brightness(int delta, QImage origin)
{
    QImage *newImage = new QImage(origin.width(), origin.height(),QImage::Format_ARGB32);

    QColor oldColor;
    int r, g, b;

    for (int i=0; i<newImage->width(); ++i)
    {
        for (int j=0; j<newImage->height(); ++j)
        {
            oldColor = QColor(origin.pixel(i,j));

            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue() + delta;

            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            newImage->setPixel(i,j, qRgb(r,g,b));
        }
    }
    return *newImage;
}

/*****************************************************************************
 *                                   水平翻转
 * 算法:
 * 左右任意一点替换为width-i  竖直方向不变
 * **************************************************************************/
QImage Algorithm::Horizontal(const QImage &origin)
{
    QImage *newImage = new QImage(QSize(origin.width(), origin.height()),QImage::Format_ARGB32);

    QColor tmpColor;
    int r, g, b;
    for (int i=0; i<newImage->width(); ++i)
    {
        for (int j=0; j<newImage->height(); ++j)
        {
            tmpColor = QColor(origin.pixel(i, j));

            r = tmpColor.red();
            g = tmpColor.green();
            b = tmpColor.blue();

            //左右置换
            newImage->setPixel(newImage->width()-i-1,j, qRgb(r,g,b));
        }
    }
    return *newImage;
}

/*****************************************************************************
 *                                   垂直翻转
 * 同理水平翻转
 * **************************************************************************/
QImage Algorithm::Vertical(const QImage &origin)
{
    QImage *newImage = new QImage(QSize(origin.width(), origin.height()),
                                  QImage::Format_ARGB32);
    QColor tmpColor;
    int r, g, b;
    for (int i=0; i<newImage->width(); ++i)
    {
        for (int j=0; j<newImage->height(); ++j)
        {
            tmpColor = QColor(origin.pixel(i, j));
            r = tmpColor.red();
            g = tmpColor.green();
            b = tmpColor.blue();

            newImage->setPixel(i, newImage->height()-j-1, qRgb(r,g,b));
        }
    }
    return *newImage;
}


/*****************************************************************************
 *                            添加相框
 * 算法:
 * 1.将边框设置到和图片一样的大小
 * 2.使用drawImage
 * **************************************************************************/
QImage Algorithm::DrawFrame(QImage origin, QImage &frame)
{
    QImage *newImage = new QImage(origin);
    QPainter painter;

    //设置边框和原图片一样大小
    QImage tmpFrame = frame.scaled(QSize(origin.width(), origin.height()));

    painter.begin(newImage);
    //自带处理函数
    painter.drawImage(0, 0, tmpFrame);
    painter.end();

    return *newImage;
}

/*****************************************************************************
 *                               简单平滑处理
 * kernel矩阵决定计算中相邻像素的影响程度
 * 然后使用滤波器进行计算
 * eg:
 *R = 20 102 99
 *    150 200 77
 *    170 210 105
 *Kenel = 0 2 0
 *        2 5 2
 *        0 2 0
 * r = ( (102*2) + (150*2) + (200*5) + (77*2) + (210*2) ) / (2+2+5+2+2) = 159
 * **************************************************************************/
QImage Algorithm::SimpleSmooth(const QImage &origin)
{
    QImage *newImage = new QImage(origin);

    int kernel[5][5] = {
        {0,0,1,0,0},
        {0,1,3,1,0},
        {1,3,7,3,1},
        {0,1,3,1,0},
        {0,0,1,0,0}
    };
    int startKernel=2;
    int sumKernel=27;
    int r,g,b;
    QColor color;

    //起始点第3个像素点 结束点是倒数第3个像素点
    for(int x=startKernel; x<newImage->width()-startKernel; ++x)
    {
        for (int y=startKernel; y<newImage->height()-startKernel; ++y)
        {
            r = g = b = 0;
            for (int i=-startKernel; i<=startKernel; ++i)
            {
                for (int j=-startKernel; j<=startKernel; ++j)
                {
                    color = QColor(origin.pixel(x+i,y+j));
                    r += color.red()*kernel[startKernel+i][startKernel+j];
                    g += color.green()*kernel[startKernel+i][startKernel+j];
                    b += color.blue()*kernel[startKernel+i][startKernel+j];
                }
            }
            r = qBound(0, r/sumKernel, 255);
            g = qBound(0, g/sumKernel, 255);
            b = qBound(0, b/sumKernel, 255);

            newImage->setPixel(x,y,qRgb(r,g,b));

        }
    }
    return *newImage;
}


/*****************************************************************************
 *                                   中值滤波
 * 实现原理:
 * 首先新建一个workImage
 * 宽: originImage.width+radius*2
 * 高: originImage.height+radiis*2
 *
 * 然后将原图拷贝到中间位置，再将图片边缘像素填充多出来的radius
 *
 * 最后 将每个radius的像素点排序，将 中值像素 作为 更新图像素
 * return
 * **************************************************************************/
QImage Algorithm::MeidaFilter(const QImage &origin, int filterRadius)
{
    int imageHeight = origin.height();
    int imageWidth = origin.width();
    //一个对象
    MedianFilter medianFilter;
    //一维数组
    int* resImageBits = new int[unsigned(imageHeight) * unsigned(imageWidth)];
    //调用applyMedianFilter()
    medianFilter.applyMedianFilter(reinterpret_cast<int*>(const_cast<uchar*>(origin.bits())), resImageBits, imageHeight, imageWidth, filterRadius);
    //
    QImage destImage(reinterpret_cast<uchar*>(resImageBits), imageWidth, imageHeight, origin.format());

    return destImage;
}


/*****************************************************************************
 *                                   锐化
 * 处理模糊图片,提升细节
 * 算法:
 *     需要卷积滤波器  用kernel矩阵 相邻像素值为-（负）
 *     pixel(中间) += pixel[i][j]*kernel[i][j]
 * **************************************************************************/
QImage Algorithm::LaplaceSharpen(const QImage &origin)
{

    int width = origin.width();
    int height = origin.height();
    QImage newImage = QImage(width, height,QImage::Format_RGB888);
    //kernel矩阵
    int kernel[3][3] = {
        {0,-1, 0},
        {-1,5,-1},
        {0,-1, 0}
    };
    int r, g, b;
    int startKernel=1;
    int sumKernel=1;
    //接下来类似于简单平滑处理
    for (int x=startKernel; x<width-startKernel; ++x)
    {
        for(int y=startKernel; y<height-startKernel; ++y)
        {
            r=g=b=0;
            //对周围像素处理
            for(int i=-startKernel; i<= startKernel; ++i)
                for(int j=-startKernel; j<=startKernel; ++j)
                {
                    r += QColor(origin.pixel(x+i,y+i)).red()*kernel[startKernel+i][startKernel+j];
                    g += QColor(origin.pixel(x+i,y+i)).green()*kernel[startKernel+i][startKernel+j];
                    b += QColor(origin.pixel(x+i,y+i)).blue()*kernel[startKernel+i][startKernel+j];
                }
            r = qBound(0, r/sumKernel, 255);
            g = qBound(0, g/sumKernel, 255);
            b = qBound(0, b/sumKernel, 255);

            newImage.setPixel(x,y, qRgb(r, g, b));
        }
    }
    return newImage;
}

/*****************************************************************************
 *                                 二值化
 * 灰色处理后，大于128的都变白，其他弄黑
 * **************************************************************************/
QImage Algorithm::Binaryzation(const QImage &origin)
{
    int width = origin.width();
    int height = origin.height();
    QImage newImg = QImage(width, height, QImage::Format_RGB888);

    for (int i=0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            //使用自带像素点判断（快速）
            int gray = qGray(origin.pixel(i,j));

            gray=gray>128?255:0;
            newImg.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return newImg;
}


/*****************************************************************************
 *                                 轮廓提取
 * 查找没有red构成的点 将其设为轮廓(黑色由b&g混合)
 * 继续搜索这个点周围8个点，如果也没有红色，那么这个点不为轮廓
 * **************************************************************************/
QImage Algorithm::ContourExtraction(const QImage &origin)
{
    int width = origin.width();
    int height = origin.height();
    int sum;
    QImage binImg = Binaryzation(origin);
    QImage newImg = QImage(width, height, QImage::Format_RGB888);
    newImg.fill(Qt::white);
    //遍历
    for(int i=1; i<width-1; ++i)
    {
        for(int j=1; j<height-1; ++j)
        {
            sum=0;
            if (QColor(binImg.pixel(i,j)).red() == 0)
            {
                newImg.setPixel(i, j, qRgb(0,0,0));
                //查找周围8个点
                for(int x=-1;x<2;++x){
                    for(int y=-1;y<2;++y){
                        sum += QColor(binImg.pixel(i+x,j+y)).red();
                    }
                }
                if (sum==0)
                    newImg.setPixel(i,j,qRgb(255,255,255));
            }
        }
    }
    return newImg;
}

/*****************************************************************************
 *                                 腐蚀
 * 用3x3的结构元素，扫描图像的每一个像素
 * 这里是先判断是否属于腐蚀区域，然后判断是否应该腐蚀
 * **************************************************************************/
QImage Algorithm::Dilate(const QImage &origin){
    int width = origin.width();
    int height = origin.height();
    QImage newImg = QImage(width, height, QImage::Format_RGB888);

    int dilateItem[3][3] = {{1,0,1},
                            {0,0,0},
                            {1,0,1}};

    for (int i=1; i<width-1; ++i)
    {
        for(int j=1; j<height-1; ++j)
        {
            newImg.setPixel(i,j,qRgb(0,0,0));//设为黑色
            for(int x=-1; x<2; ++x)
            {
                for(int y=-1; y<2; ++y)
                {
                    if(dilateItem[x+1][y+1] == 1)
                        continue;
                    QColor mColor = origin.pixel(i+x,j+y);
                    if(mColor.red() > 128){
                        newImg.setPixel(i,j,qRgb(255,255,255));
                    }
                }
            }
        }
    }
    return newImg;
}

/*****************************************************************************
 *                                 膨胀
 * 膨胀的原理和腐蚀相反
 * **************************************************************************/

QImage Algorithm::Expansion(const QImage &origin)
{
    int width = origin.width();
    int height = origin.height();
    QImage newImg = QImage(width, height, QImage::Format_RGB888);

    int dilateItem[3][3] = {{1,0,1},
                            {0,0,0},
                            {1,0,1}};

    for (int i=1; i<width-1; ++i)
    {
        for(int j=1; j<height-1; ++j)
        {
            newImg.setPixel(i,j,qRgb(255,255,255));//设为黑色
            for(int x=-1; x<2; ++x)
            {
                for(int y=-1; y<2; ++y)
                {
                    if(dilateItem[x+1][y+1] == 1)
                        continue;
                    QColor mColor = origin.pixel(i+x,j+y);
                    if(mColor.red() < 128){
                        newImg.setPixel(i,j,qRgb(0,0,0));
                    }
                }
            }
        }
    }
    return newImg;
}


/*****************************************************************************
 *                                开运算
 * 先腐蚀后膨胀
 * **************************************************************************/
QImage Algorithm::Opening(const QImage &origin)
{
    QImage afterDilate = Dilate(origin);
    QImage afterExpansion = Expansion(afterDilate);

    return afterExpansion;
}

/*****************************************************************************
 *                                闭运算
 * 先膨胀后腐蚀
 * **************************************************************************/
QImage Algorithm::Closing(const QImage &origin)
{
    QImage afterExpansion = Expansion(origin);
    QImage afterDilate = Dilate(afterExpansion);

    return afterDilate;
}
