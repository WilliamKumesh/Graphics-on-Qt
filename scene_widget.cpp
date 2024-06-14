#include "scene_widget.h"
#include <QFile>

namespace
{
    const QString filename = "C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\Chess.obj";
    const QString filenameBoard = "C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\ChessBoard.obj";
    const QString filenameKing = "C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\King.obj";
    const QString filenameKnight = "C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\Knight.obj";
    const QString filenamePawn = "C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\Pawn.obj";
    const QString filenameQueen = "C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\Queen.obj";
    const QString filenameBishop = "C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\Bishop.obj";
    const QString filenameRook = "C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\Rook.obj";
}

Scene_widget::Scene_widget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setWindowState(Qt::WindowFullScreen);

    QVector3D view_pos {fieldSize * (float)3.5, 20, -50};
    m_camera = Camera(view_pos);

    initQueue();
}

void Scene_widget::initQueue()
{
    queue_steps.push_back(Step(QPair(3,1), QPair(3,3), fieldSize));
    queue_steps.push_back(Step(QPair(3,6), QPair(3,4), fieldSize));
    queue_steps.push_back(Step(QPair(1,0), QPair(2,2), fieldSize));
    queue_steps.push_back(Step(QPair(6,7), QPair(5,5), fieldSize));
    queue_steps.push_back(Step(QPair(2,0), QPair(5,3), fieldSize));
    queue_steps.push_back(Step(QPair(2,7), QPair(5,4), fieldSize));
    queue_steps.push_back(Step(QPair(5,1), QPair(5,2), fieldSize));
    queue_steps.push_back(Step(QPair(1,7), QPair(2,5), fieldSize));
    queue_steps.push_back(Step(QPair(4,1), QPair(4,3), fieldSize));
    queue_steps.push_back(Step(QPair(3,4), QPair(4,3), fieldSize));
    queue_steps.push_back(Step(QPair(5,2), QPair(4,3), fieldSize));
}

void Scene_widget::updateFigures()
{
    verticesWhite.clear();

    for(int i = 0; i < figures.size()/2; i++)
    {
        verticesWhite.append(figures[i].vertices);
    }

    verticesBlack.clear();

    for(int i = figures.size()/2; i < figures.size(); i++)
    {
        verticesBlack.append(figures[i].vertices);
    }

    update();
}

void Scene_widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }

    m_camera_controller.key_press_event(event);

    if(event->key() == Qt::Key_Right)
    {
        if(queue_steps.empty())
        {
            return;
        }

        auto step = queue_steps[0];

        Figure* checked = nullptr;

        for(auto& it: figures)
        {
            if(step.getStart().first == it.getBoardCoord(fieldSize).first && step.getStart().second == it.getBoardCoord(fieldSize).second)
            {
                it.move(step.makeStep());
                checked = &it;
                history_steps.push_back(step);
                queue_steps.erase(queue_steps.cbegin());
                updateFigures();
                step_num++;
                break;
            }
        }

        if(checked != nullptr)
        {
            int pos = 0;
            for(auto& it: figures)
            {
                if(checked->getBoardCoord(fieldSize).first == it.getBoardCoord(fieldSize).first
                    && checked->getBoardCoord(fieldSize).second == it.getBoardCoord(fieldSize).second && checked != &it)
                {
                    deleted_figures.emplace(deleted_figures.cbegin(), QPair(Figure(it), QPair(step_num, pos)));
                    figures.erase(figures.cbegin() + pos);
                    updateFigures();
                    break;
                }
                pos++;
            }
        }
    }

    if(event->key() == Qt::Key_Left)
    {
        if(history_steps.empty())
        {
            return;
        }

        auto step = history_steps[history_steps.size()-1];

        for(auto& it: figures)
        {
            if(step.getEnd().first == it.getBoardCoord(fieldSize).first && step.getEnd().second == it.getBoardCoord(fieldSize).second)
            {
                it.move(-step.makeStep());
                queue_steps.emplace(queue_steps.cbegin(), step);
                history_steps.erase(history_steps.cend()-1);

                if(!deleted_figures.empty())
                {
                    if(step_num == deleted_figures[0].second.first)
                    {
                        figures.emplace(figures.cbegin() + deleted_figures[0].second.second, deleted_figures[0].first);
                        deleted_figures.erase(deleted_figures.cbegin());
                    }
                }

                step_num--;
                updateFigures();
                break;
            }
        }
    }
}

void Scene_widget::keyReleaseEvent(QKeyEvent *event)
{
    m_camera_controller.key_released_event(event);
}

void Scene_widget::mouseMoveEvent(QMouseEvent *event)
{
    m_camera.mouseMoveEvent(event);
}

void Scene_widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_camera.setLastPos(QCursor::pos());
        m_camera.setAbleMove(true);
    }
}

void Scene_widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_camera.setAbleMove(false);
    }
}

QVector<Vertex> Scene_widget::load_mesh(QString file_path, bool format)
{
    QVector<Vertex> vertices;

    QFile file(file_path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open file" << file_path;
        return vertices;
    }

    QVector<QVector3D> positions;
    QVector<QVector2D> tex_coords;
    QVector<QVector3D> normals;

    while (!file.atEnd())
    {
        QString line = file.readLine().trimmed();
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);

        if (parts.isEmpty())
            continue;



        if (parts[0] == "v")
        {
            positions.append(QVector3D(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
        }
        else if (parts[0] == "vt")
        {
            tex_coords.append(QVector2D(parts[1].toFloat(), parts[2].toFloat()));
        }
        else if (parts[0] == "vn")
        {
            normals.append(QVector3D(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
        }
        else if(parts[0] == "f" && !format)
        {
            for (int i = 1; i <= 4; ++i)
            {
                QStringList vertex_indices = parts[i].split("/");
                int pos_index = vertex_indices[0].toInt() - 1;
                int normal_index = vertex_indices[2].toInt() - 1;

                Vertex vertex;
                vertex.position = positions[pos_index];
                vertex.normal = normals[normal_index];
                vertices.append(vertex);
            }
        }
    }

    file.close();
    return vertices;
}

QVector<Vertex> Scene_widget::initFigures(bool white)
{
    auto vertexK = load_mesh(filenameKing);
    auto vertexKn = load_mesh(filenameKnight);
    auto vertexQ = load_mesh(filenameQueen);
    auto vertexB = load_mesh(filenameBishop);
    auto vertexP = load_mesh(filenamePawn);
    auto vertexR = load_mesh(filenameRook);

    Figure rookFigure1;
    Figure rookFigure2;
    Figure knight1Figure;
    Figure knight2Figure;
    Figure bishop1Figure;
    Figure bishop2Figure;
    Figure kingFigure;
    Figure queenFigure;

    Figure pawn1;
    Figure pawn2;
    Figure pawn3;
    Figure pawn4;
    Figure pawn5;
    Figure pawn6;
    Figure pawn7;
    Figure pawn8;

    if(white)
    {
        for(int i = 0; i < vertexB.count(); i++)
        {
            vertexB[i].normal = -vertexB[i].normal;
        }

        for(int i = 0; i < vertexR.count(); i++)
        {
            vertexR[i].normal = -vertexR[i].normal;
        }

        rookFigure1 = Figure(vertexR, QVector3D(0, 0, 0));
        rookFigure2 = Figure(vertexR, QVector3D(7*fieldSize, 0, 0));
        knight1Figure = Figure(vertexKn, QVector3D(1*fieldSize, 0, 0));
        knight2Figure = Figure(vertexKn, QVector3D(6*fieldSize, 0, 0));
        bishop1Figure = Figure(vertexB, QVector3D(2*fieldSize, 0, 0));
        bishop2Figure = Figure(vertexB, QVector3D(5*fieldSize, 0, 0));
        kingFigure = Figure(vertexK, QVector3D(3*fieldSize, 0, 0));
        queenFigure = Figure(vertexQ, QVector3D(4*fieldSize, 0, 0));

        pawn1 = Figure(vertexP, QVector3D(0, 0, -fieldSize));
        pawn2 = Figure(vertexP, QVector3D(1*fieldSize, 0, -fieldSize));
        pawn3 = Figure(vertexP, QVector3D(2*fieldSize, 0, -fieldSize));
        pawn4 = Figure(vertexP, QVector3D(3*fieldSize, 0, -fieldSize));
        pawn5 = Figure(vertexP, QVector3D(4*fieldSize, 0, -fieldSize));
        pawn6 = Figure(vertexP, QVector3D(5*fieldSize, 0, -fieldSize));
        pawn7 = Figure(vertexP, QVector3D(6*fieldSize, 0, -fieldSize));
        pawn8 = Figure(vertexP, QVector3D(7*fieldSize, 0, -fieldSize));
    }

    if(!white)
    {
        for(int i = 0; i < vertexKn.count(); i++)
        {
            vertexKn[i].position.setX(-vertexKn[i].position.x());
            vertexKn[i].position.setZ(-vertexKn[i].position.z());
        }

        rookFigure1 = Figure(vertexR, QVector3D(0, 0, -7*fieldSize));
        rookFigure2 = Figure(vertexR, QVector3D(7*fieldSize, 0, -7*fieldSize));
        knight1Figure = Figure(vertexKn, QVector3D(1*fieldSize, 0, -7*fieldSize));
        knight2Figure = Figure(vertexKn, QVector3D(6*fieldSize, 0, -7*fieldSize));
        bishop1Figure = Figure(vertexB, QVector3D(2*fieldSize, 0, -7*fieldSize));
        bishop2Figure = Figure(vertexB, QVector3D(5*fieldSize, 0, -7*fieldSize));
        kingFigure = Figure(vertexK, QVector3D(3*fieldSize, 0, -7*fieldSize));
        queenFigure = Figure(vertexQ, QVector3D(4*fieldSize, 0, -7*fieldSize));

        pawn1 = Figure(vertexP, QVector3D(0, 0, -6*fieldSize));
        pawn2 = Figure(vertexP, QVector3D(1*fieldSize, 0, -6*fieldSize));
        pawn3 = Figure(vertexP, QVector3D(2*fieldSize, 0, -6*fieldSize));
        pawn4 = Figure(vertexP, QVector3D(3*fieldSize, 0, -6*fieldSize));
        pawn5 = Figure(vertexP, QVector3D(4*fieldSize, 0, -6*fieldSize));
        pawn6 = Figure(vertexP, QVector3D(5*fieldSize, 0, -6*fieldSize));
        pawn7 = Figure(vertexP, QVector3D(6*fieldSize, 0, -6*fieldSize));
        pawn8 = Figure(vertexP, QVector3D(7*fieldSize, 0, -6*fieldSize));
    }

    figures.push_back(rookFigure1);
    figures.push_back(knight1Figure);
    figures.push_back(bishop1Figure);
    figures.push_back(kingFigure);
    figures.push_back(queenFigure);
    figures.push_back(bishop2Figure);
    figures.push_back(knight2Figure);
    figures.push_back(rookFigure2);

    figures.push_back(pawn1);
    figures.push_back(pawn2);
    figures.push_back(pawn3);
    figures.push_back(pawn4);
    figures.push_back(pawn5);
    figures.push_back(pawn6);
    figures.push_back(pawn7);
    figures.push_back(pawn8);

    QVector<Vertex> allVertices;
    allVertices.append(rookFigure1.vertices);
    allVertices.append(knight1Figure.vertices);
    allVertices.append(bishop1Figure.vertices);
    allVertices.append(kingFigure.vertices);
    allVertices.append(queenFigure.vertices);
    allVertices.append(bishop2Figure.vertices);
    allVertices.append(knight2Figure.vertices);
    allVertices.append(rookFigure2.vertices);

    allVertices.append(pawn1.vertices);
    allVertices.append(pawn2.vertices);
    allVertices.append(pawn3.vertices);
    allVertices.append(pawn4.vertices);
    allVertices.append(pawn5.vertices);
    allVertices.append(pawn6.vertices);
    allVertices.append(pawn7.vertices);
    allVertices.append(pawn8.vertices);

    return allVertices;
}

void Scene_widget:: resizeGL(int w, int h)
{

}

void Scene_widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1,1,1,1);
    glEnable(GL_DEPTH_TEST);

    m_shader = new QOpenGLShaderProgram(this);
    m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\vertex.glsl");
    m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\fragment.glsl");

    QImage textureB("C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\textureBlack.jpg");
    textureBlack = new QOpenGLTexture(textureB.mirrored());
    textureBlack->setWrapMode(QOpenGLTexture::ClampToEdge);
    textureBlack->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureBlack->setMagnificationFilter(QOpenGLTexture::Linear);
    textureBlack->generateMipMaps();

    QImage textureW("C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\textureWhite.jpg");

    textureWhite = new QOpenGLTexture(textureW.mirrored());
    textureWhite->setWrapMode(QOpenGLTexture::ClampToEdge);
    textureWhite->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureWhite->setMagnificationFilter(QOpenGLTexture::Linear);
    textureWhite->generateMipMaps();

    verticesWhite = initFigures();
    verticesBlack = initFigures(false);

    boardVertices = load_mesh(filenameBoard);

    connect(this, &Scene_widget::frameSwapped, [=]() { update(); m_camera_controller.update(m_camera);});
}

void Scene_widget::paintGL()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader->bind();

    QMatrix4x4 view;
    QMatrix4x4 model;
    QMatrix4x4 projection;

    static size_t frame { 0 };
    frame++;

    projection.perspective(60.0f, (width() * retinaScale) / (height() * retinaScale), 0.1f, 100.0f);

    m_shader->setUniformValue("view", m_camera.view());
    m_shader->setUniformValue("model", model);
    m_shader->setUniformValue("projection", projection);
    m_shader->setUniformValue("view_pos", m_camera.position());
    m_shader->setUniformValue("tex", 1);

    // Создаем и заполняем вершинный буфер
    m_vertices.create();
    m_vertices.bind();
    m_vertices.allocate((verticesWhite.size() + verticesBlack.size() + boardVertices.size()) * sizeof(Vertex));
    m_vertices.write(0, verticesWhite.constData(), verticesWhite.size() * sizeof(Vertex));
    m_vertices.write(verticesWhite.size() * sizeof(Vertex), verticesBlack.constData(), verticesBlack.size() * sizeof(Vertex));
    m_vertices.write((verticesWhite.size() + verticesBlack.size()) * sizeof(Vertex), boardVertices.constData(), boardVertices.size() * sizeof(Vertex));
    m_vertices.bind();

    // Отрисовка белых фигур
    textureWhite->bind(1);
    m_shader->enableAttributeArray(0);
    m_shader->enableAttributeArray(1);
    m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
    m_shader->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
    glDrawArrays(GL_QUADS, 0, verticesWhite.size());
    m_shader->disableAttributeArray(0);
    m_shader->disableAttributeArray(1);
    textureWhite->release();

    // Отрисовка черных фигур
    textureBlack->bind(1);
    m_shader->enableAttributeArray(0);
    m_shader->enableAttributeArray(1);
    m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
    m_shader->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
    glDrawArrays(GL_QUADS, verticesWhite.size(), verticesBlack.size());
    m_shader->disableAttributeArray(0);
    m_shader->disableAttributeArray(1);
    textureBlack->release();

    // Отрисовка доски
    textureWhite->bind(1);
    m_shader->enableAttributeArray(0);
    m_shader->enableAttributeArray(1);
    m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
    m_shader->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
    glDrawArrays(GL_QUADS, verticesWhite.size() + verticesBlack.size(), boardVertices.size());
    m_shader->disableAttributeArray(0);
    m_shader->disableAttributeArray(1);
    textureWhite->release();

    // Освобождение ресурсов
    m_vertices.release();
    m_shader->release();
}
