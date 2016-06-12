
#include <QtGui>
#include "frameeditorscene.h"
#include "utils.h"
#include "settings.h"


FrameScene::FrameScene(QWidget *parent) : QGraphicsScene(parent)
{
    //enforce custom coordinate system [-1.0, 1.0]
    //Y is negative to make positive values go upwards
    setSceneRect(-3.0, -3.0, 6.0, 6.0); //bigger than (-1.0, -1.0, 2.0, 2.0) for scrollability
    setBackgroundBrush(Qt::black);
    addItem(grid = new Grid);
    reverse = false;
}

void FrameScene::setModel(Frame* m)
{
    foreach(Path* path, paths)
    {
        removeItem(path);
        delete path;
    }

    paths.clear();

    //TODO: disconnect signals from the model
    model = m;
    //TODO connect signals

    //create all of the path objects in the model
    for(int i = 0; i < model->rowCount(); i++)
    {
        QModelIndex index = model->index(i);
        lzr::Frame lzr_path = index.data().value<lzr::Frame>();

        Path* path = new Path(index, lzr_path);
        paths.append(path);
        addItem(path);

        connect(path, SIGNAL(changed(Path*)),
                this, SLOT(path_changed(Path*)));
    }
}

void FrameScene::setPathSelection(QItemSelectionModel* path_sel)
{
    path_selection = path_sel;
}

void FrameScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect); //because we always render the entire scene

    if(tool == LINE && current_path())
    {
        //lookup the currently selected path
        Path* path = current_path();
        Point* point;

        if(!reverse)
            point = path->last();
        else
            point = path->first();

        QPointF pos = grid->constrain_and_maybe_snap(mouse);
        painter->setPen(QPen(Qt::darkGray, 0));
        painter->drawLine(QLineF(point->x(), point->y(), pos.x(), pos.y()));
    }
}

void FrameScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    mouse = e->scenePos();

    if(tool == LINE && current_path())
    {
        update();
    }

    QGraphicsScene::mouseMoveEvent(e);
}

void FrameScene::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    if(tool == LINE && current_path())
    {
        Path* path = current_path();
        Point* old_point = path->last();
        QPointF pos = grid->constrain_and_maybe_snap(e->scenePos());
        path->add_point(new Point(pos, old_point->getColor()), reverse);
    }

    QGraphicsScene::mousePressEvent(e);
}

void FrameScene::keyPressEvent(QKeyEvent* e)
{
    if(!e->isAutoRepeat())
    {
        switch(e->key())
        {
            case EDITOR_SNAP_KEY:
                grid->set_snapping(true);
                update();
                break;
            case EDITOR_REVERSE_KEY:
                reverse = true;
                update();
                break;
        }
    }

    QGraphicsScene::keyPressEvent(e);
}

void FrameScene::keyReleaseEvent(QKeyEvent* e)
{
    if(!e->isAutoRepeat())
    {
        switch(e->key())
        {
            case EDITOR_SNAP_KEY:
                grid->set_snapping(false);
                update();
                break;
            case EDITOR_REVERSE_KEY:
                reverse = false;
                update();
                break;
        }
    }

    QGraphicsScene::keyReleaseEvent(e);
}

Path* FrameScene::current_path()
{
    if(path_selection->hasSelection())
        return paths[path_selection->currentIndex().row()];
    return NULL;
}


/*
 * Slots
 */

void FrameScene::path_changed(Path* path)
{
    //update the model with the new path data
    QVariant v;
    v.setValue(path->to_LZR());
    model->setData(path->get_index(), v);
}

void FrameScene::tool_changed(tool_t t)
{
    tool = t;
    update();
}

void FrameScene::color_changed(QColor c)
{
    color = c;
}

void FrameScene::grid_changed(int divisions)
{
    grid->set_divisions(divisions);
}