#include "Figures/Headers/Polygon.h"
#include "Figures/Headers/Polyline.h"

Polyline::Polyline(QPolygonF polygon, bool* editMode, bool* removeMode, QGraphicsItem *parent) :
    FigureInterface(polygon, editMode, removeMode, parent) {
    menu.addAction("Lenght", SLOT(getLength()));
    Polyline::applyCenter();
}

QPointF Polyline::masscenter() {
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

void Polyline::moveVertices(QPointF startpoint, QPointF endpoint) {
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
    prepareGeometryChange();
    boundRect = QRect(xmin-10, ymin-10, xmax-xmin+20, ymax-ymin+20);
}

void Polyline::removeVertices(QPointF point) {
    QGraphicsScene* scene = this->scene();
    if (polygon.size()>2) {
        int numofpoints = polygon.size();
        int closestindex = 0;
        qreal closest = pointfrompoint(polygon[0], point);
        for (int i=1; i<numofpoints; i++) {
            if (closest > pointfrompoint(polygon[i], point)) {
                closest = pointfrompoint(polygon[i], point);
                closestindex = i;
            }
        }
        QPolygonF newPolygon;
        for (int i = numofpoints-1; i>closestindex; i--) {
            newPolygon.push_back(polygon.back());
            polygon.pop_back();
        }
        polygon.pop_back();
        FigureInterface* tmp;
        if (newPolygon.size()>1) {
            tmp = new Polyline(newPolygon, editMode, removeMode);
            tmp->setTransform(this->transform());
            tmp->applyTransform();
            tmp->applyCenter();
            scene->addItem(tmp);
        }
        if (polygon.size()>1) {
            tmp = new Polyline(polygon, editMode, removeMode);
            tmp->setTransform(this->transform());
            tmp->applyTransform();
            tmp->applyCenter();
            scene->addItem(tmp);
        }
    }
    scene->removeItem(this);
    delete this;
}

// sets item's local coordinate center to it masscenter (for rotate and resize to work correctly)
void Polyline::applyCenter() {
    if (!polygon.size())
        return;
    qreal numofpoints = polygon.size();
    QPointF center;
    for (int i=0; i<numofpoints; i++)
        center+=polygon[i];
    center/=numofpoints;
    polygon.translate(-center);
    Matrix = QTransform().translate(center.x(), center.y())*Matrix;
    this->setTransform(Matrix, false);
    recalculateBound();
}

void Polyline::paint(QPainter*  ppainter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (hasFocus())
        ppainter->setPen(Qt::yellow);
    ppainter->drawPolyline(polygon);
}

//length function
void Polyline::getLength() {
    QPolygonF scenePolygon = this->mapToScene(polygon);
    int numofpoints = scenePolygon.size()-1;
    qreal result = 0;
    for (int i=0; i<numofpoints; i++)
        result += pointfrompoint(scenePolygon[(i+1)%numofpoints], scenePolygon[i]);
    QMessageBox box;
    box.setText(QString::number(result));
    box.exec();
}
