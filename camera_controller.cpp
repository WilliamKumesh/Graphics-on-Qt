#include "camera_controller.h"

namespace {
    constexpr double sens = 0.02/2;
}

void Camera_controller::key_press_event(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        m_moving_direct = true;
        break;
    case Qt::Key_A:
        m_moving_left = true;
        break;
    case Qt::Key_S:
        m_moving_back = true;
        break;
    case Qt::Key_D:
        m_moving_right = true;
        break;
    case Qt::Key_Shift:
        m_moving_down = true;
        break;
    case Qt::Key_Space:
        m_moving_up = true;
        break;
    }
}

void Camera_controller::key_released_event(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        m_moving_direct = false;
        break;
    case Qt::Key_A:
        m_moving_left = false;
        break;
    case Qt::Key_S:
        m_moving_back = false;
        break;
    case Qt::Key_D:
        m_moving_right = false;
        break;
    case Qt::Key_Shift:
        m_moving_down = false;
        break;
    case Qt::Key_Space:
        m_moving_up = false;
        break;
    }
}

void Camera_controller::update(Camera &camera)
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last_update).count();

    QVector3D offset { 0, 0, 0};
    offset[0] += m_moving_right ? -duration * sens : 0;
    offset[0] += m_moving_left ? duration * sens : 0;
    offset[2] += m_moving_direct ? duration * sens : 0;
    offset[2] += m_moving_back ? -duration * sens : 0;
    offset[1] += m_moving_up ? duration * sens : 0;
    offset[1] += m_moving_down ? -duration * sens : 0;

    camera.move(std::move(offset));
    m_last_update = now;
}
