#include "SceneView.h"

void SceneView::wheelEvent(QWheelEvent *event) {
    qreal scaleFactor = 1+(qreal)event->angleDelta().y()/8/360;
    this->setTransform(QTransform().scale(scaleFactor, scaleFactor), true);
}

void SceneView::addCircle() {
    Scene* ptr = dynamic_cast<Scene*>(this->scene());
    if (!ptr)
        return;
    QDialog wgt;
    QLineEdit *radius = new QLineEdit;
    QLineEdit *vertices = new QLineEdit;
    radius->setValidator(new QDoubleValidator(0,0001, 1e9, radius));
    vertices->setValidator(new QIntValidator(3, 1000, vertices));
    QPushButton *button = new QPushButton("Ok");
    QObject::connect(button, SIGNAL(clicked()), &wgt, SLOT(close()));
    QGridLayout *layout = new QGridLayout(&wgt);
    layout->addWidget(new QLabel("Radius: "), 0, 0);
    layout->addWidget(radius, 1, 0);
    layout->addWidget(new QLabel("Number of vertices: "), 0, 1);
    layout->addWidget(vertices, 1, 1);
    layout->addWidget(button, 2, 1);
    wgt.exec();
    ptr->addCircle(vertices->text().toUInt(), radius->text().toDouble());
}

void SceneView::addRect() {
    Scene* ptr = dynamic_cast<Scene*>(this->scene());
    if (!ptr)
        return;
    QDialog wgt;
    QLineEdit *a = new QLineEdit;
    QLineEdit *b = new QLineEdit;
    a->setValidator(new QDoubleValidator(0,0001, 1e9, a));
    b->setValidator(new QDoubleValidator(0,0001, 1e9, b));
    QPushButton *button = new QPushButton("Ok");
    QObject::connect(button, SIGNAL(clicked()), &wgt, SLOT(close()));
    QGridLayout *layout = new QGridLayout(&wgt);
    layout->addWidget(new QLabel("X-size: "), 0, 0);
    layout->addWidget(a, 1, 0);
    layout->addWidget(new QLabel("Y-size; "), 0, 1);
    layout->addWidget(b, 1, 1);
    layout->addWidget(button, 2, 1);
    wgt.exec();
    ptr->addRectangle(a->text().toDouble(), b->text().toDouble());
}

void SceneView::saveImageToFile() {
    dynamic_cast<Scene*>(this->scene())->saveImageToFile(QFileDialog::getSaveFileName(nullptr, tr("Save File"), "", tr("*.png")));
}

void SceneView::saveRowToFile() {
    dynamic_cast<Scene*>(this->scene())->saveRowToFile(QFileDialog::getSaveFileName(nullptr, tr("Save File"), "", tr("*.graph")));
}

void SceneView::readFromFile() {
    dynamic_cast<Scene*>(this->scene())->readFromFile(QFileDialog::getOpenFileName(nullptr, tr("Open File"), "", tr("*.graph")));
}

void SceneView::setBackgroundColor() {
    QColorDialog colorDialog;
    colorDialog.exec();
    this->scene()->setBackgroundBrush(colorDialog.selectedColor());
}

void SceneView::paintMode(bool a) {
    Scene* ptr = dynamic_cast<Scene*>(this->scene());
    if (!ptr)
        return;
    if (a)
        ptr->paintingEnable();
    else
        ptr->paintingDisable();
}
