#ifndef POLYGON_H
#define POLYGON_H

#include "Figures/Headers/FigureInterface.h"

class Polygon : public FigureInterface {
    Q_OBJECT
protected:
    QBrush brush;
public:
    Polygon(QPolygonF polygon, bool* editMode = nullptr, bool* removeMode = nullptr, QTransform Matrix = QTransform(), QGraphicsItem *parent = nullptr);
    void removeVertices(QPointF point);
    void paint(QPainter*  ppainter, const QStyleOptionGraphicsItem*, QWidget*);
    ~Polygon() = default;
public slots:
    void setBrush();
    void getS();
    void getP();
};

#endif // POLYGON_H
