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
	// Creates a Node 
    Node(int id, int x, int y, int size);

	// Destructor
    ~Node();

	// Returns visible shape of the node
    QGraphicsItem *GetShape() const;

	// Returns position of the drawn shape
    QPointF *GetPosition() const;

	// Returns the ID of the node
    int GetId() const;

	// Checks if the node is a wall
    bool IsWall() const;

	// Checks if the node is a start node
    bool IsStart() const;

	// Checks if the node is a goal
    bool IsGoal() const;

	// Check if a node has been visited
	bool WasVisited() const;

	// Gets the previously visited node
	Node *GetPreviousNode() const;

	// Sets the descriptive text of the node shape to the current node ID
    void SetDescription() const;

	// Sets the current node as a starting point
    void SetStart(bool start);

	// Sets the current node as the goal
    void SetGoal(bool goal);

	// Set the previously visited node pointer
	void SetPreviousNode(Node *node);

	// Sets the current node as visited
	void SetVisited(bool visited);

	// Sets the node to a wall node
    void SetWall();

	// Remove wall from a node
    void UnsetWall();

	// Highlights the node 
	void TracePath() const;
private:
	// Size of the node's shape
    int m_shapeSize;

	// Unique integer ID of the node
    int m_id;

	// Shape representation of the node
    QGraphicsRectItem *m_shape;

	// Node shape position
    QPointF *m_position;

	// Node text description
    QGraphicsTextItem *m_desc;

	// Pointer for search algorithms to find the previous node
	Node *m_previous;

	// Flags to determine status of a node
    bool m_wall;
    bool m_start;
    bool m_goal;
	bool m_visited;
};

