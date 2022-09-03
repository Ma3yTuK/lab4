#ifndef RELATIONPOINT_H
#define RELATIONPOINT_H

#include <QtWidgets>

class RelationPoint : public QGraphicsItem {
    QPointF relationPoint;
    QPen pen;
    const qreal penWidth;
public:
    RelationPoint(qreal x = 0, qreal y = 0, QGraphicsItem* parent = nullptr) : QGraphicsItem(parent), relationPoint(x, y), pen(Qt::red), penWidth(3) {pen.setCosmetic(true), pen.setWidth(penWidth);}
    QRectF boundingRect() const {return QRectF(relationPoint.x()-penWidth, relationPoint.y()-penWidth, penWidth, penWidth);}
    void setRelationPoint(QPointF point) {prepareGeometryChange(); relationPoint = point;}
    QPointF getRelationPoint() {return relationPoint;}
    void paint(QPainter*  ppainter, const QStyleOptionGraphicsItem*, QWidget*);
    ~RelationPoint() = default;
};

#endif // RELATIONPOINT_H
