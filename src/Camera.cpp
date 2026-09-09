#include "Camera.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Camera::Camera() {
    reset();
}

void Camera::reset() {
    distance = 6.0f;
    yaw = 45.0f;
    pitch = 30.0f;
    panX = 0.0f;
    panY = 0.0f;
    isDraggingRotate = false;
    isDraggingPan = false;
    lastMouseX = 0;
    lastMouseY = 0;
    autoRotate = true;
    autoRotateSpeed = 25.0f; // degrees per second
}

void Camera::apply3D() const {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Pan translation
    glTranslatef(panX, panY, -distance);

    // Rotation
    glRotatef(pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);
}

void Camera::apply2D(int width, int height) const {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-width / 2.0, width / 2.0, -height / 2.0, height / 2.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Camera::onMouseDown(int x, int y, int button) {
    lastMouseX = x;
    lastMouseY = y;
    if (button == 0) { // Left click
        isDraggingRotate = true;
    } else if (button == 1) { // Right click
        isDraggingPan = true;
    }
}

void Camera::onMouseUp(int button) {
    if (button == 0) isDraggingRotate = false;
    if (button == 1) isDraggingPan = false;
}

void Camera::onMouseMove(int x, int y) {
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    if (isDraggingRotate) {
        yaw += dx * 0.5f;
        pitch += dy * 0.5f;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    if (isDraggingPan) {
        panX += dx * 0.01f;
        panY -= dy * 0.01f;
    }

    lastMouseX = x;
    lastMouseY = y;
}

void Camera::onMouseWheel(int delta) {
    distance -= (delta / 120.0f) * 0.6f;
    if (distance < 1.0f) distance = 1.0f;
    if (distance > 50.0f) distance = 50.0f;
}

void Camera::update(float deltaTime) {
    if (autoRotate && !isDraggingRotate) {
        yaw += autoRotateSpeed * deltaTime;
        if (yaw >= 360.0f) yaw -= 360.0f;
    }
}
