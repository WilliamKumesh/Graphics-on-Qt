#ifndef FIGURE_H
#define FIGURE_H
#include <QVector3D>
#include <QVector>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QPair>

struct Vertex
{
    QVector3D position;
    QVector3D normal;
};

class Figure
{

public:

    QVector<Vertex> vertices;
    QVector3D position; // Позиция фигуры в пространстве

    Figure()
    {

    }

    Figure(const QVector<Vertex>& meshVertices, const QVector3D& pos)
        : vertices(meshVertices), position(pos)
    {
        // Сдвиг вершин фигуры на заданную позицию
        for (auto& vertex : vertices)
        {
            vertex.position += position;
        }
    }

    void move(const QVector3D& pos)
    {
        position += pos;
        for (auto& vertex : vertices)
        {
            vertex.position += pos;
        }
    }

    QPair<int,int> getBoardCoord(float fieldSize)
    {
        return {position.x() / fieldSize, -position.z() / fieldSize};
    }
};

#endif // FIGURE_H
