#ifndef STEP_H
#define STEP_H
#include <QPair>
#include <QVector3D>

class Step
{
    QPair<int, int> startPos;
    QPair<int, int> endPos;
    QVector3D step;
public:
    Step(QPair<int, int> start_pos, QPair<int, int> end_pos, float fieldSize)
    {
        startPos = start_pos;
        endPos = end_pos;
        step = {(end_pos.first - start_pos.first) * fieldSize, 0, -(end_pos.second - start_pos.second) * fieldSize};
    }

    const QPair<int, int>& getStart()
    {
        return startPos;
    }

    const QPair<int, int>& getEnd()
    {
        return endPos;
    }

    const QVector3D& makeStep()
    {
        return step;
    }
};

#endif // STEP_H
