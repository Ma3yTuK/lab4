#include "Figures/Headers/RelationPoint.h"

void RelationPoint::paint(QPainter*  ppainter, const QStyleOptionGraphicsItem*, QWidget*) {
    ppainter->setPen(pen);
    ppainter->drawPoint(relationPoint);
}
