#ifndef POLYLINE_H
#define POLYLINE_H

#include "Figures/Headers/FigureInterface.h"

class Polyline : public FigureInterface {
public:
    Polyline(QPolygonF polygon, bool* editMode = nullptr, bool* removeMode = nullptr, QGraphicsItem *parent = nullptr);
    QPointF masscenter();
    void moveVertices(QPointF startpoint, QPointF endpoint);
    void removeVertices(QPointF point);
    void applyCenter();
    void paint(QPainter*  ppainter, const QStyleOptionGraphicsItem*, QWidget*);
    ~Polyline() = default;
public slots:
    void getLength();
};

#endif // POLYLINE_H
