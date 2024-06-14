#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <QKeyEvent>
#include "camera.h"

class Camera_controller final {
public:
    void key_press_event(QKeyEvent *event);

    void key_released_event(QKeyEvent *event);

    void update(Camera &camera);

private:
    bool m_moving_right { false };
    bool m_moving_left { false };
    bool m_moving_direct { false };
    bool m_moving_back { false };
    bool m_moving_up { false };
    bool m_moving_down { false };

    std::chrono::time_point<std::chrono::high_resolution_clock> m_last_update;
};

#endif // CAMERA_CONTROLLER_H
