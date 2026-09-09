#ifndef CAMERA_H
#define CAMERA_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Camera {
public:
    Camera();

    void reset();
    void apply3D() const;
    void apply2D(int width, int height) const;

    void onMouseMove(int x, int y);
    void onMouseDown(int x, int y, int button);
    void onMouseUp(int button);
    void onMouseWheel(int delta);
    void update(float deltaTime);

    // Camera parameters
    float distance;
    float yaw;
    float pitch;
    float panX;
    float panY;

    bool isDraggingRotate;
    bool isDraggingPan;
    int lastMouseX;
    int lastMouseY;

    bool autoRotate;
    float autoRotateSpeed;
};

#endif // CAMERA_H
