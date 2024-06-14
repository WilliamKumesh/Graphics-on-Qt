#ifndef SCENE_WIDGET_H
#define SCENE_WIDGET_H

#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QOpenGLBuffer>
#include <QGuiApplication>
#include <QLineEdit>
#include <vector>
#include "Camera.h"
#include "camera_controller.h"
#include "Figure.h"
#include "Step.h"

class Scene_widget final : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    [[nodiscard]]
    explicit Scene_widget(QWidget *parent = nullptr);

    static QVector<Vertex> load_mesh(QString file_path, bool format = false);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;


    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QVector<Vertex> initFigures(bool white = true);
    void initQueue();

    float fieldSize = 5;
    int step_num = 0;

    void updateFigures();

    Camera m_camera;
    Camera_controller m_camera_controller;

    std::vector<Figure> figures;
    std::vector<QPair<Figure, QPair<int,int>>> deleted_figures;
    std::vector<Step> queue_steps;
    std::vector<Step> history_steps;

    QOpenGLShaderProgram *m_shader { nullptr };

    QOpenGLBuffer m_vertices;

    QVector<Vertex> boardVertices;
    QVector<Vertex> verticesWhite;
    QVector<Vertex> verticesBlack;

    QOpenGLTexture *textureBlack;
    QOpenGLTexture *textureWhite;
};

#endif // SCENE_WIDGET_H
