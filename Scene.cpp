#include <Scene.h>

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (tmpPolygon) {
        QPolygonF tmp = tmpPolygon->getPolygon();
        if (tmp.size())
            tmp.pop_back();
        tmp.push_back(mouseEvent->scenePos());
        tmp.push_back(mouseEvent->scenePos());
        tmpPolygon->setPolygon(tmp);
    }
    transformMode = rotateMode = scaleMode = false;
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void Scene::focusOutEvent(QFocusEvent *focusEvent) {
    if (tmpPolygon) {
        QPolygonF tmp = tmpPolygon->getPolygon();
        if (tmp.size()>2) {
            tmp.pop_back();
            tmpPolygon->setPolygon(tmp);
            tmpPolygon->applyCenter();
            tmpPolygon = new Polyline(QPolygonF(), &editMode, &removeMode);
        }
        else {
            tmpPolygon->setPolygon(QPolygonF());
        }
        return;
    }
    transformMode = rotateMode = scaleMode = editMode = removeMode = false;
    QGraphicsScene::focusOutEvent(focusEvent);
}

void Scene::keyPressEvent(QKeyEvent *keyEvent) {
    if (tmpPolygon) {
        QPolygonF tmp = tmpPolygon->getPolygon();
        if (tmp.size()>2) {
            tmp.pop_back();
            tmpPolygon->setPolygon(tmp);
            tmpPolygon->applyCenter();
            tmpPolygon = new Polyline(QPolygonF(), &editMode, &removeMode);
        }
        else {
            tmpPolygon->setPolygon(QPolygonF());
        }
        return;
    }
    if (transformMode || rotateMode || scaleMode || editMode || removeMode)
        return;
    switch (keyEvent->key()) {
    case Qt::Key_G:
        if (dynamic_cast<FigureInterface*>(this->focusItem()))
            transformMode = true;
        break;
    case Qt::Key_R:
        if (dynamic_cast<FigureInterface*>(this->focusItem()))
            rotateMode = true;
        break;
    case Qt::Key_S:
        if (dynamic_cast<FigureInterface*>(this->focusItem()))
            scaleMode = true;
        break;
    case Qt::Key_E:
        editMode = true;
        break;
    case Qt::Key_D:
        removeMode = true;
        break;
    default:
        QGraphicsScene::keyPressEvent(keyEvent);
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    FigureInterface* item = dynamic_cast<FigureInterface*>(this->focusItem());
    QPointF pos = mouseEvent->scenePos();
    QPointF lastPos = mouseEvent->lastScenePos();
    if(tmpPolygon) {
        QPolygonF tmp = tmpPolygon->getPolygon();
        if(tmp.size()) {
            tmp.pop_back();
            tmp.push_back(mouseEvent->scenePos());
            tmpPolygon->setPolygon(tmp);
        }
    }
    else if(transformMode) {
        item->movex(pos.x()-lastPos.x());
        item->movey(pos.y()-lastPos.y());
        item->applyTransform();
    }
    else if(rotateMode) {
        qreal r = pos.x()-relationPoint.x();
        qreal a = pos.y()-lastPos.y();
        item->rotatefrompoint(relationPoint, a/r);
        item->applyTransform();
    }
    else if(scaleMode) {
        item->resizefrompointx(relationPoint, 1+(pos.x()-lastPos.x())/(lastPos.x()-relationPoint.x()));
        item->resizefrompointy(relationPoint, 1+(pos.y()-lastPos.y())/(lastPos.y()-relationPoint.y()));
        item->applyTransform();
    }
    else {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void Scene::paintingEnable() {
    transformMode = rotateMode = scaleMode = editMode = removeMode = false;
    tmpPolygon = new Polyline(QPolygonF(), &editMode, &removeMode);
    this->addItem(tmpPolygon);
}

void Scene::paintingDisable() {
    QPolygonF tmp = tmpPolygon->getPolygon();
    if(tmp.size()<3) {
        removeItem(tmpPolygon);
        delete tmpPolygon;
    }
    else {
        tmp.pop_back();
        tmpPolygon->setPolygon(tmp);
        tmpPolygon->applyCenter();
    }
    tmpPolygon = nullptr;
}

void Scene::addCircle(int n, qreal radius) {
    QPolygonF polygon;
    for (int i=0; i<n+1; i++)
        polygon << QPointF(radius*qCos(i*2*M_PI/n), radius*qSin(i*2*M_PI/n));
    addItem(new Polygon(polygon, &editMode, &removeMode));
}

void Scene::addRectangle(qreal a, qreal b) {
    QPolygonF polygon;
    polygon << QPointF(-a/2, -b/2) << QPointF(-a/2, b/2) << QPointF(a/2, b/2) << QPointF(a/2, -b/2) << QPointF(-a/2, -b/2);
    addItem(new Polygon(polygon, &editMode, &removeMode));
}

void Scene::saveImageToFile(QString path) {
    clearFocus();
    clearSelection();
    QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    render(&painter);
    image.save(path);
}
