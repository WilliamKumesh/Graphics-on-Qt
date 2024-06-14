#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QGuiApplication>
#include <QCursor>
#include <iostream>>

class Camera final {
public:
    explicit Camera(QVector3D position) noexcept;

    explicit Camera() noexcept;

    const QVector3D &position() const noexcept;

    QMatrix4x4 view() const;

    void position(QVector3D position) noexcept;

    void move(QVector3D offset) noexcept;

    void setLastPos(QPoint pos)
    {
        m_lastMousePos = pos;
    }

    void setAbleMove(bool able)
    {
        able_move = able;
    }

    void mouseMoveEvent(QMouseEvent *event)
    {
        if(able_move)
        {
            QPoint currentMousePos = event->globalPos();

            QPoint delta = currentMousePos - m_lastMousePos;

            m_lastMousePos = currentMousePos;

            float rotationSpeed = 0.11f;

            m_direction = QQuaternion::fromAxisAndAngle(m_direction_normal, delta.x() * rotationSpeed) * m_direction;

            QVector3D right = QVector3D::crossProduct(m_direction, m_direction_normal);
            m_direction = QQuaternion::fromAxisAndAngle(right, delta.y() * rotationSpeed) * m_direction;

            m_direction.normalize();
        }
    }

private:
    bool able_move = false;
    QPoint m_lastMousePos;
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_direction_normal;
};

#endif // CAMERA_H
