#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QtWidgets>

class PopUpWindow : public QMessageBox {
    Q_OBJECT
public:
    PopUpWindow(QWidget* parent = nullptr) : QMessageBox(parent) {}
    ~PopUpWindow() = default;
};

#endif // POPUPWINDOW_H
