#include "Figures/Headers/FigureInterface.h"

//returns distance between two points
qreal pointfrompoint(QPointF point1, QPointF point2) {
    return qSqrt(qPow(point1.x()-point2.x(), 2) + qPow(point1.y()-point2.y(), 2));
}

FigureInterface::FigureInterface(QPolygonF polygon, bool* editMode, bool* removeMode, QTransform Matrix, QGraphicsItem *parent) :
    QGraphicsObject(parent), polygon(polygon), editMode(editMode), removeMode(removeMode), Matrix(Matrix), pen(Qt::black) {
    pen.setCosmetic(true);
    setTransform(Matrix);
    recalculateBound();
    setFlag(QGraphicsObject::ItemIsFocusable);
}

QPointF FigureInterface::masscenter() {
    if (!polygon.size())
        return QPointF();
    QPolygonF localPolygon = this->mapToScene(polygon);
    qreal numofpoints = localPolygon.size();
    QPointF result;
    for (int i=0; i<numofpoints; i++) {
        result+=localPolygon[i];
    }
    return result/=numofpoints;
}

void FigureInterface::moveVertices(QPointF endpoint) {
    if (!polygon.size())
        return;
    int numofpoints = polygon.size();
    int closestindex = 0;
    int xmax, xmin, ymax, ymin;
    xmax = xmin = endpoint.x();
    ymax = ymin = endpoint.y();
    qreal closest = pointfrompoint(polygon[0], startpoint);
    for (int i=0; i<numofpoints; i++) {
        if (closest > pointfrompoint(polygon[i], startpoint)) {
            closest = pointfrompoint(polygon[i], startpoint);
            closestindex = i;
        }
        if (polygon[i].x()<xmin)
            xmin = polygon[i].x();
        if (polygon[i].x()>xmax)
            xmax = polygon[i].x();
        if (polygon[i].y()>ymax)
            ymax = polygon[i].y();
        if (polygon[i].y()<ymin)
            ymin = polygon[i].y();
    }
    polygon[closestindex] = endpoint;
    startpoint = endpoint;
    prepareGeometryChange();
    boundRect = QRectF(xmin-10, ymin-10, xmax-xmin+20, ymax-ymin+20);
}

void FigureInterface::mousePressEvent(QGraphicsSceneMouseEvent* pe) {
    if (removeMode && *removeMode)
        removeVertices(pe->pos());
    else if (editMode && *editMode)
        startpoint = pe->pos();
    else if (pe->button() == Qt::RightButton)
        menu.exec(pe->screenPos());
    else
        QGraphicsItem::mousePressEvent(pe);
}

void FigureInterface::mouseMoveEvent(QGraphicsSceneMouseEvent* pe) {
    if (editMode && *editMode)
        moveVertices(pe->pos());
    QGraphicsItem::mouseMoveEvent(pe);
}

void FigureInterface::focusInEvent(QFocusEvent *event) {
    pen.setColor(Qt::yellow);
    update();
    QGraphicsItem::focusInEvent(event);
}

void FigureInterface::focusOutEvent(QFocusEvent *event) {
    pen.setColor(Qt::black);
    update();
    QGraphicsItem::focusOutEvent(event);
}

//this item regenerates area for item to be painted inside
void FigureInterface::recalculateBound() {
    if(!polygon.size()) {
        prepareGeometryChange();
        boundRect = QRectF();
        return;
    }
    qreal xmax, xmin, ymax, ymin;
    xmax = xmin = polygon[0].x();
    ymax = ymin = polygon[0].y();
    int numofpoints = polygon.size();
    for (int i=1; i<numofpoints; i++) {
        if (polygon[i].x()<xmin)
            xmin = polygon[i].x();
        if (polygon[i].x()>xmax)
            xmax = polygon[i].x();
        if (polygon[i].y()>ymax)
            ymax = polygon[i].y();
        if (polygon[i].y()<ymin)
            ymin = polygon[i].y();
    }
    prepareGeometryChange();
    boundRect = QRectF(xmin-10, ymin-10, xmax-xmin+20, ymax-ymin+20);
}

void FigureInterface::movex(qreal localx) {
    this->setTransform(Matrix*QTransform().translate(localx, 0), false);
}

void FigureInterface::movey(qreal localy) {
    this->setTransform(Matrix*QTransform().translate(0, localy), false);
}

//rotates figure around point
void FigureInterface::rotatefrompoint(QPointF relationPoint, qreal angle) {
    QTransform pointMatrix = QTransform().translate(relationPoint.x(), relationPoint.y());
    this->setTransform(Matrix*pointMatrix.inverted()*QTransform().rotateRadians(angle)*pointMatrix, false);
}

//scales figure along x axis related to point
void FigureInterface::resizefrompointx(QPointF relationPoint, qreal scalex) {
    if (scalex<=0)
        scalex = 1;
    QTransform pointMatrix = QTransform().translate(relationPoint.x(), relationPoint.y());
    this->setTransform(Matrix*pointMatrix.inverted()*QTransform().scale(scalex, 1)*pointMatrix, false);
}

//scales figure along y axis related to point
void FigureInterface::resizefrompointy(QPointF relationPoint, qreal scaley) {
    if (scaley<=0)
        scaley = 1;
    QTransform pointMatrix = QTransform().translate(relationPoint.x(), relationPoint.y());
    this->setTransform(Matrix*pointMatrix.inverted()*QTransform().scale(1, scaley)*pointMatrix, false);
}
