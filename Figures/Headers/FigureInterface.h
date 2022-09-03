#ifndef FIGUREINTERFACE_H
#define FIGUREINTERFACE_H

#include <QtWidgets>
#include "PopUpWindow.h"
#include "RClickMenu.h"

qreal pointfrompoint(QPointF point1, QPointF point2);

class FigureInterface : public QGraphicsObject {
    Q_OBJECT
protected:
    QPolygonF polygon;
    QRectF boundRect;
    RClickMenu menu;
    PopUpWindow window;
    bool* editMode;
    bool* removeMode;
    QTransform Matrix;
    QPen pen;
    QPointF startpoint;
    void mousePressEvent(QGraphicsSceneMouseEvent* pe);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* pe);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
public:
    FigureInterface(QPolygonF polygon, bool* editMode, bool* removeMode, QTransform Matrix, QGraphicsItem *parent);
    QRectF boundingRect() const {return boundRect;}
    QPointF masscenter();
    void setPolygon(QPolygonF polygon) {this->polygon = polygon; recalculateBound();}
    QPolygonF getPolygon() {return polygon;}
    void moveVertices(QPointF endpoint);
    virtual void removeVertices(QPointF point) = 0;
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
