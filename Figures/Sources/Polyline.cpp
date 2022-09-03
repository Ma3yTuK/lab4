#include "Figures/Headers/Polygon.h"
#include "Figures/Headers/Polyline.h"

Polyline::Polyline(QPolygonF polygon, bool* editMode, bool* removeMode, QTransform Matrix, QGraphicsItem *parent) :
    FigureInterface(polygon, editMode, removeMode, Matrix, parent) {
    menu.addAction("Length", this, SLOT(getLength()));
    menu.addAction("Connect", this, SLOT(toPolygon()));
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
            tmp = new Polyline(newPolygon, editMode, removeMode, Matrix);
            tmp->setTransform(this->transform());
            tmp->applyTransform();
            scene->addItem(tmp);
        }
        if (polygon.size()>1) {
            tmp = new Polyline(polygon, editMode, removeMode, Matrix);
            tmp->setTransform(this->transform());
            tmp->applyTransform();
            scene->addItem(tmp);
        }
    }
    scene->removeItem(this);
    delete this;
}

void Polyline::paint(QPainter* ppainter, const QStyleOptionGraphicsItem*, QWidget*) {
    ppainter->setPen(pen);
    ppainter->drawPolyline(polygon);
}

//length function
void Polyline::getLength() {
    QPolygonF scenePolygon = this->mapToScene(polygon);
    int numofpoints = scenePolygon.size()-1;
    qreal result = 0;
    for (int i=0; i<numofpoints; i++)
        result += pointfrompoint(scenePolygon[(i+1)%numofpoints], scenePolygon[i]);
    window.setText(QString::number(result));
    window.exec();
}

void Polyline::toPolygon() {
    if (polygon.size()>2) {
        scene()->addItem(new Polygon(polygon, editMode, removeMode, Matrix));
        scene()->removeItem(this);
        deleteLater();
    }
}
