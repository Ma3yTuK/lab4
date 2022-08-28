#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include "Scene.h"

class SceneView : public QGraphicsView {
    Q_OBJECT
protected:
    void wheelEvent(QWheelEvent *event);
public:
    SceneView(QWidget *parent = nullptr) : QGraphicsView(parent) {this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);}
    ~SceneView() = default;
public slots:
    void addCircle();
    void addRect();
    void removeItem();
    void saveImageToFile();
    void saveRowToFile();
    void readFromFile();
    void setBackgroundColor();
    void paintMode(bool a);
};

#endif // SCENEVIEW_H
