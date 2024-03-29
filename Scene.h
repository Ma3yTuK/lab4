#ifndef SCENE_H
#define SCENE_H

#include "Figures/Headers/Polygon.h"
#include "Figures/Headers/Polyline.h"
#include "Figures/Headers/RelationPoint.h"

class Scene : public QGraphicsScene {
    Q_OBJECT
protected:
    RelationPoint* relationPoint;
    bool transformMode;
    bool rotateMode;
    bool scaleMode;
    bool editMode;
    bool removeMode;
    bool setPointMode;
    FigureInterface* tmpPolygon;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void focusOutEvent(QFocusEvent *focusEvent);
    void keyPressEvent(QKeyEvent *keyEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
public:
    Scene(QObject* parent = nullptr) : QGraphicsScene(parent), relationPoint(new RelationPoint), transformMode(false), rotateMode(false),
        scaleMode(false), editMode(false), removeMode(false), setPointMode(false), tmpPolygon(nullptr) {addItem(relationPoint);}
    void paintingEnable();
    void paintingDisable();
    void addCircle(int n, qreal radius);
    void addRectangle(qreal a, qreal b);
    void saveImageToFile(QString path);
    void saveRowToFile(QString path);
    void readFromFile(QString path);
    ~Scene() = default;
};

#endif // SCENE_H
