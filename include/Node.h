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
	bool WasVisited() const;
	Node *GetPreviousNode() const;
    void SetDescription() const;
    void SetStart(bool start);
    void SetGoal(bool goal);
	void SetActive(bool active);
	void SetPreviousNode(Node *node);
	void SetVisited(bool visited);
    void SetWall();
    void UnsetWall();
	void TracePath();
private:
    int m_shapeSize;
    int m_id;
    QGraphicsRectItem *m_shape;
    QPointF *m_position;
    QGraphicsTextItem *m_desc;
	Node *m_previous;
    bool m_wall;
    bool m_start;
    bool m_goal;
	bool m_visited;
};

