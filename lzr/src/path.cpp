
#include "path.h"


Path::Path(QModelIndex i, lzr::Frame frame) : QGraphicsObject(0)
{
    index = i;

    for(lzr::Point lzr_point : frame)
    {
        own_point(new Point(lzr_point), points.size());
    }
}

Point* Path::first()
{
    return points.front();
}

Point* Path::last()
{
    return points.back();
}

void Path::add_point(Point* point, bool add_at_front)
{
    if(add_at_front)
        own_point(point, 0);
    else
        own_point(point, points.size());

    emit changed(this);
}

lzr::Frame Path::to_LZR() const
{
    lzr::Frame frame;
    foreach(Point* item, points)
    {
        Point* point = (Point*) item;
        frame.add(point->to_LZR());
    }

    return frame;
}

QModelIndex Path::get_index()
{
    return index;
}

void Path::point_changed()
{
    emit changed(this);
}

void Path::own_point(Point* point, int where)
{
    points.insert(where, point);
    point->setParentItem(this);
    connect(point, SIGNAL(changed()),
            this, SLOT(point_changed()));
}

QRectF Path::boundingRect() const
{
    return QRectF(-1,-1,2,2); //the whole screen/grid
}

void Path::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    for(int i = 0; i < points.size() - 1; i++)
    {
        const Point* p1 = points[i];
        const Point* p2 = points[i+1];
        QLineF line(p1->x(), p1->y(), p2->x(), p2->y());
        painter->setPen(QPen(p2->getColor(), 0));
        painter->drawLine(line);
    }
}