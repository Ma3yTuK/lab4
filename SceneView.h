#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include "Scene.h"

class SceneView : public QGraphicsView {
    Q_OBJECT
protected:
    void wheelEvent(QWheelEvent *event);
    void enterEvent(QEvent *event) {setFocus(), QWidget::enterEvent(event);}
    void leaveEvent(QEvent *event) {clearFocus(), QWidget::leaveEvent(event);}
public:
    SceneView(QWidget *parent = nullptr) : QGraphicsView(parent) {this->setMouseTracking(true);}
    ~SceneView() = default;
public slots:
    void addCircle();
    void addRect();
    void saveImageToFile();
    void saveRowToFile();
    void readFromFile();
    void setBackgroundColor();
    void paintMode(bool a);
};

#endif // SCENEVIEW_H
