#include "Figures/Headers/Polygon.h"
#include "Figures/Headers/Polyline.h"

Polygon::Polygon(QPolygonF polygon, bool* editMode, bool* removeMode, QGraphicsItem *parent) :
    FigureInterface(polygon, editMode, removeMode, parent) {
    menu.addAction("Square", SLOT(getS()));
    menu.addAction("Perimetr", SLOT(getP()));
    menu.addAction("Set brush", SLOT(setBrush()));
    Polygon::applyCenter();
}

QPointF Polygon::masscenter() {
    if (polygon.size()<2)
        return QPointF();
    QPolygonF localPolygon = this->mapToScene(polygon);
    qreal numofpoints = localPolygon.size()-1;
    QPointF result;
    for (int i=0; i<numofpoints; i++) {
        result+=localPolygon[i];
    }
    return result/=numofpoints;
}

void Polygon::moveVertices(QPointF startpoint, QPointF endpoint) {
    if (polygon.size()<2)
        return;
    int numofpoints = polygon.size()-1;
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
    if (!closestindex)
        polygon[numofpoints]=endpoint;
    polygon[closestindex] = endpoint;
    prepareGeometryChange();
    boundRect = QRect(xmin-10, ymin-10, xmax-xmin+20, ymax-ymin+20);
}

void Polygon::removeVertices(QPointF point) {
    QGraphicsScene* scene = this->scene();
    if (polygon.size()>3) {
        polygon.pop_back();
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
        for (int i = 0; i<numofpoints-1; i++)
            newPolygon[i] = polygon[(i+1+closestindex)%numofpoints];
        FigureInterface* tmp = new Polyline(newPolygon, editMode, removeMode);
        tmp->setTransform(this->transform());
        tmp->applyTransform();
        tmp->applyCenter();
        scene->addItem(tmp);
    }
    scene->removeItem(this);
    delete this;
}

// sets item's local coordinate center to it masscenter (for rotate and resize to work correctly)
void Polygon::applyCenter() {
    if (polygon.size()<2)
        return;
    qreal numofpoints = polygon.size()-1;
    QPointF center;
    for (int i=0; i<numofpoints; i++)
        center+=polygon[i];
    center/=numofpoints;
    polygon.translate(-center);
    Matrix = QTransform().translate(center.x(), center.y())*Matrix;
    this->setTransform(Matrix, false);
    recalculateBound();
}

void Polygon::paint(QPainter*  ppainter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (hasFocus())
        ppainter->setPen(Qt::yellow);
    ppainter->setBrush(brush);
    ppainter->drawPolyline(polygon);
}

void Polygon::setBrush() {
    QColorDialog colorDialog;
    colorDialog.exec();
    brush.setColor(colorDialog.selectedColor());
    update();
}

//returns y-coordinate of crossing of two lines
qreal crosspointy(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3) {
    return (y2-y1)*(x1-x3)/(x1-x2)+y1;
}

// checks if point L is under line ab (for next function)
bool lineishigher(QPointF L, QPointF a, QPointF b) {
    if((a.y()-L.y())/qAbs(L.x()-a.x()) > (b.y()-L.y())/qAbs(L.x()-b.x()))
        return true;
    return false;
}

//returns how many times vertical line polygon[pointindex].x() crosses other
//edges of polygon under polygon[pointindex].y() coordinate (for next function)
int vertundercrossings (QPolygonF polygon, int pointindex, bool position = false) {
    int numofpoints = polygon.size()-1;
    int result=0;
    qreal currentjx, currentjy, lastjy, lastjx, xvalue, yvalue, maxv, minv;
    xvalue = polygon[pointindex].x();
    yvalue = polygon[pointindex].y();
    lastjy = polygon[numofpoints-1].y();
    lastjx = polygon[numofpoints-1].x();
    for (int j=0; j<numofpoints; j++) {
        currentjy = polygon[j].y();
        currentjx = polygon[j].x();
        if (xvalue == currentjx) {
            int k=j, l=numofpoints+j, pos0, pos1;
            while(polygon[++k%numofpoints].x()==xvalue);
            while(polygon[--l%numofpoints].x()==xvalue);
            if (polygon[--k%numofpoints].y() > polygon[++l%numofpoints].y())
                maxv = polygon[k++%numofpoints].y(), minv = polygon[l--%numofpoints].y();
            else
                maxv = polygon[l--%numofpoints].y(), minv = polygon[k++%numofpoints].y();
            if (position)
                pos1 = k, pos0 = l;
            else
                pos1 = l, pos0 = k;
            if (maxv == currentjy) {
                if(polygon[l%numofpoints].x() < xvalue == xvalue > polygon[k%numofpoints].x()) {
                    if (maxv==minv && maxv==yvalue && lineishigher(polygon[(l+1)%numofpoints], polygon[pos1%numofpoints], polygon[pos0%numofpoints]))
                        result++;
                    else if (maxv!=minv && minv == yvalue)
                        result++;
                }
                else if (polygon[l%numofpoints].x() < xvalue == xvalue < polygon[k%numofpoints].x() && yvalue>=minv) {
                    result++;
                }
            }
        }
        else if (xvalue!=lastjx && lastjx < xvalue == xvalue < currentjx &&
                 yvalue>crosspointy(currentjx, currentjy, lastjx, lastjy, xvalue)-1) {
            result++;
        }
        lastjx = currentjx;
        lastjy = currentjy;
    }
    return result;
}

// square function
void Polygon::getS() {
    QPolygonF scenePolygon = this->mapToScene(polygon);
    int numofpoints = scenePolygon.size()-1;
    qreal result = 0;
    for (int i=0; i<numofpoints; i++) {
        if (!(vertundercrossings(scenePolygon, i)%2))
            result+=qAbs((scenePolygon[(i+1)%numofpoints].x()-scenePolygon[i].x()))*(scenePolygon[i].y()+scenePolygon[(i+1)%numofpoints].y())/2;
        else
            result-=qAbs((scenePolygon[(i+1)%numofpoints].x()-scenePolygon[i].x()))*(scenePolygon[i].y()+scenePolygon[(i+1)%numofpoints].y())/2;
    }
    QMessageBox box;
    box.setText(QString::number(result));
    box.exec();
}

//perimetr function
void Polygon::getP() {
    QPolygonF scenePolygon = this->mapToScene(polygon);
    int numofpoints = scenePolygon.size()-1;
    qreal result = 0;
    for (int i=0; i<numofpoints; i++)
        result += pointfrompoint(scenePolygon[(i+1)%numofpoints], scenePolygon[i]);
    QMessageBox box;
    box.setText(QString::number(result));
    box.exec();
}
