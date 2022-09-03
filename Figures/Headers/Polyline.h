#ifndef POLYLINE_H
#define POLYLINE_H

#include "Figures/Headers/FigureInterface.h"

class Polyline : public FigureInterface {
    Q_OBJECT
public:
    Polyline(QPolygonF polygon, bool* editMode = nullptr, bool* removeMode = nullptr, QTransform Matrix = QTransform(), QGraphicsItem *parent = nullptr);
    void removeVertices(QPointF point);
    void paint(QPainter*  ppainter, const QStyleOptionGraphicsItem*, QWidget*);
    ~Polyline() = default;
public slots:
    void getLength();
    void toPolygon();
};

#endif // POLYLINE_H
