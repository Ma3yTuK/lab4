#ifndef FIGUREINTERFACE_H
#define FIGUREINTERFACE_H

#include <QtWidgets>

qreal pointfrompoint(QPointF point1, QPointF point2);

class FigureInterface : public QGraphicsObject {
protected:
    QPolygonF polygon;
    QRectF boundRect;
    QMenu menu;
    bool* editMode;
    bool* removeMode;
    QTransform Matrix;
    void mousePressEvent(QGraphicsSceneMouseEvent* pe);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* pe);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pe);
public:
    FigureInterface(QPolygonF polygon, bool* editMode, bool* removeMode, QTransform Matrix, QGraphicsItem *parent);
    QRectF boundingRect() const {return boundRect;}
    virtual QPointF masscenter() = 0;
    void setPolygon(QPolygonF polygon) {this->polygon = polygon; update();}
    QPolygonF getPolygon() {return polygon;}
    virtual void moveVertices(QPointF startpoint, QPointF endpoint) = 0;
    virtual void removeVertices(QPointF point) = 0;
    virtual void applyCenter() = 0;
    void recalculateBound();
    void applyTransform() {Matrix=this->transform();}
    void movex(qreal localx);
    void movey(qreal localy);
    void rotatefrompoint(QPointF relationPoint, qreal angle);
    void resizefrompointx(QPointF relationPoint, qreal scalex);
    void resizefrompointy(QPointF relationPoint, qreal scaley);
    ~FigureInterface() = default;
};

#endif // FIGUREINTERFACE_H
