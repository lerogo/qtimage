#include "graphicsview.h"

//重写QGraphicsView实现图像的缩放

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
}

GraphicsView::~GraphicsView()
{
}

//使得鼠标能够实现放大缩小
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    int distance = e->delta()/66;
    double val;

    if (distance != 0)
    {
        val = pow(1.2, distance);
        this->scale(val, val);
    }
}
