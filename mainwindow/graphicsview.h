#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QtMath>

//重写QGraphicsView实现图像的缩放
class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget* parent = nullptr);
    ~GraphicsView();

protected:
    virtual void wheelEvent(QWheelEvent *e);
};

#endif // GRAPHICSVIEW_H
