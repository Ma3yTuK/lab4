#ifndef RCLICKMENU_H
#define RCLICKMENU_H

#include <QtWidgets>

class RClickMenu : public QMenu {
    Q_OBJECT
public:
    RClickMenu(QWidget* parent = nullptr) : QMenu(parent) {}
    ~RClickMenu() = default;
};

#endif // RCLICKMENU_H
