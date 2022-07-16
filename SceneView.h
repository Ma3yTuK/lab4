#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <Scene.h>

class SceneView : public QGraphicsView {
    Q_OBJECT;
protected:
    void wheelEvent(QWheelEvent *event);
public:
    SceneView(Scene *scene, QWidget *parent = nullptr) : QGraphicsView(scene, parent) {}
    ~SceneView() = default;
public slots:
    void addCircle();
    void addRect();
    void removeItem();
    void setBackgroundColor();
    void paintMode(bool a);
};

#endif // SCENEVIEW_H
