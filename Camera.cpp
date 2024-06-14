#include "camera.h"

Camera::Camera(QVector3D position) noexcept :
    m_position(std::move(position)), m_direction( { 0, 0, 1 }), m_direction_normal({ 0, 1, 0 })
{
     m_lastMousePos = QCursor::pos();
}

Camera::Camera() noexcept : Camera({ 0, 0, 0})
{

}

const QVector3D &Camera::position() const noexcept
{
    return m_position;
}

QMatrix4x4 Camera::view() const
{
    QMatrix4x4 view;

    QVector3D looking_at = m_position + m_direction;
    view.lookAt(m_position, looking_at, m_direction_normal);
    return view;
}

void Camera::position(QVector3D position) noexcept
{
    m_position = std::move(position);
}

void Camera::move(QVector3D offset) noexcept
{
    m_position += offset;
}
