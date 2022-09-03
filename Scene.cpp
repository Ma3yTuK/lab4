#include "Scene.h"

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (tmpPolygon) {
        QPolygonF tmp = tmpPolygon->getPolygon();
        if (tmp.size())
            tmp.pop_back();
        tmp.push_back(mouseEvent->scenePos());
        tmp.push_back(mouseEvent->scenePos());
        tmpPolygon->setPolygon(tmp);
        return;
    }
    if (setPointMode) {
        relationPoint->setRelationPoint(mouseEvent->scenePos());
        setPointMode = false;
        return;
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
            tmpPolygon = new Polyline(QPolygonF(), &editMode, &removeMode);
            this->addItem(tmpPolygon);
        }
        else {
            tmpPolygon->setPolygon(QPolygonF());
        }
    }
    transformMode = rotateMode = scaleMode = editMode = removeMode = setPointMode = false;
    QGraphicsScene::focusOutEvent(focusEvent);
}

void Scene::keyPressEvent(QKeyEvent *keyEvent) {
    if (tmpPolygon) {
        QPolygonF tmp = tmpPolygon->getPolygon();
        if (tmp.size()>2) {
            tmp.pop_back();
            tmpPolygon->setPolygon(tmp);
            tmpPolygon = new Polyline(QPolygonF(), &editMode, &removeMode);
            this->addItem(tmpPolygon);
        }
        else {
            tmpPolygon->setPolygon(QPolygonF());
        }
        return;
    }
    if (transformMode || rotateMode || scaleMode || editMode || removeMode || setPointMode) {
        transformMode = rotateMode = scaleMode = editMode = removeMode = setPointMode = false;
        return;
    }
    QGraphicsItem* tmp = focusItem();
    switch (keyEvent->key()) {
    case Qt::Key_G:
        if (dynamic_cast<FigureInterface*>(tmp))
            transformMode = true;
        break;
    case Qt::Key_R:
        if (dynamic_cast<FigureInterface*>(tmp))
            rotateMode = true;
        break;
    case Qt::Key_S:
        if (dynamic_cast<FigureInterface*>(tmp))
            scaleMode = true;
        break;
    case Qt::Key_E:
        editMode = true;
        break;
    case Qt::Key_D:
        removeMode = true;
        break;
    case Qt::Key_X:
        if(tmp) {
            removeItem(tmp);
            delete tmp;
        }
        break;
    case Qt::Key_V:
        if (tmp) {
            removeItem(tmp);
            addItem(tmp);
        }
        break;
    case Qt::Key_P:
        setPointMode = true;
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
        if(tmp.size()>1) {
            tmp.pop_back();
            tmp.push_back(mouseEvent->scenePos());
            tmpPolygon->setPolygon(tmp);
        }
    }
    else if(transformMode) {
        item->movex(pos.x()-lastPos.x());
        item->applyTransform();
        item->movey(pos.y()-lastPos.y());
        item->applyTransform();
    }
    else if(rotateMode) {
        qreal x1 = pos.x()-relationPoint->getRelationPoint().x();
        qreal x2 = lastPos.x()-relationPoint->getRelationPoint().x();
        qreal y1 = pos.y()-relationPoint->getRelationPoint().y();
        qreal y2 = lastPos.y()-relationPoint->getRelationPoint().y();
        qreal angle = pointfrompoint(pos, lastPos)/pointfrompoint(relationPoint->getRelationPoint(), pos);
        if(x1*y2-x2*y1>0)
            item->rotatefrompoint(relationPoint->getRelationPoint(), -angle);
        else
            item->rotatefrompoint(relationPoint->getRelationPoint(), angle);
        item->applyTransform();
    }
    else if(scaleMode) {
        qreal x = lastPos.x()-relationPoint->getRelationPoint().x();
        qreal y = lastPos.y()-relationPoint->getRelationPoint().y();
        if (x)
            item->resizefrompointx(relationPoint->getRelationPoint(), 1+(pos.x()-lastPos.x())/x);
        item->applyTransform();
        if (y)
            item->resizefrompointy(relationPoint->getRelationPoint(), 1+(pos.y()-lastPos.y())/y);
        item->applyTransform();
    }
    else {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void Scene::paintingEnable() {
    transformMode = rotateMode = scaleMode = editMode = removeMode = setPointMode = false;
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
    }
    tmpPolygon = nullptr;
}

void Scene::addCircle(int n, qreal radius) {
    if (n<3 || !radius)
        return;
    QPolygonF polygon;
    for (int i=0; i<n; i++)
        polygon << QPointF(radius*qCos(i*2*M_PI/n), radius*qSin(i*2*M_PI/n));
    addItem(new Polygon(polygon, &editMode, &removeMode));
}

void Scene::addRectangle(qreal a, qreal b) {
    if (!a || !b)
        return;
    QPolygonF polygon;
    polygon << QPointF(-a/2, -b/2) << QPointF(-a/2, b/2) << QPointF(a/2, b/2) << QPointF(a/2, -b/2);
    addItem(new Polygon(polygon, &editMode, &removeMode));
}

void Scene::saveImageToFile(QString path) {
    clearFocus();
    QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    render(&painter);
    image.save(path);
}

void Scene::saveRowToFile(QString path) {
    QList<QGraphicsItem*> items = this->items();
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    for(auto &i : items) {
        if (dynamic_cast<Polygon*>(i)) {
            out << qint8(0);
        }
        else if (dynamic_cast<Polyline*>(i)) {
            out << qint8(1);
        }
        else {
            continue;
        }
        FigureInterface* item = dynamic_cast<FigureInterface*>(i);
        out << item->transform() << item->getPolygon();
    }
}

void Scene::readFromFile(QString path) {
    this->clear();
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    while(!in.atEnd()) {
        qint8 type;
        QTransform Matrix;
        QPolygonF polygon;
        in >> type >> Matrix >> polygon;
        if (in.status() == QDataStream::ReadCorruptData) {
            this->clear();
            this->addItem(relationPoint = new RelationPoint);
            return;
        }
        if (type)
            this->addItem(new Polyline(polygon, &editMode, &removeMode, Matrix));
        else
            this->addItem(new Polygon(polygon, &editMode, &removeMode, Matrix));
    }
    this->addItem(relationPoint = new RelationPoint);
}
