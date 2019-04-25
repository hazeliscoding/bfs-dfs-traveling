#pragma once

#include <QWidget>
#include <QRect>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QDebug>

class Node
{
public:
    Node(int id, int x, int y, int size);
    ~Node();

    QGraphicsItem *GetShape() const;
    QPointF *GetPosition() const;
    int GetId() const;
    bool IsWall() const;
    bool IsStart() const;
    bool IsGoal() const;
    void SetDescription() const;
    void SetStart(bool start);
    void SetGoal(bool goal);
    void SetWall();
    void UnsetWall();
private:
    int m_shapeSize;
    int m_id;
    QGraphicsRectItem *m_shape;
    QPointF *m_position;
    QGraphicsTextItem *m_desc;
    bool m_wall;
    bool m_start;
    bool m_goal;
};

