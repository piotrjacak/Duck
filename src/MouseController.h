#ifndef MOUSE_CONTROLLER_H
#define MOUSE_CONTROLLER_H

class Camera;

class MouseController {
public:
    explicit MouseController(Camera& camera);

    void onMouseButton(int button, int action);
    void onCursorPos(double xpos, double ypos);

private:
    Camera& camera;

    double lastX = 0.0;
    double lastY = 0.0;
    bool firstMove = true;
    bool lmbDown = false;
    bool rmbDown = false;
};

#endif
