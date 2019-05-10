#pragma once

#include <QWidget>
#include <QRect>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QDebug>

class Vertex;

using VertexHashShapeList = QHash<QGraphicsItem*, Vertex*>;
using VertexHashIDList = QHash<int, Vertex*>;

class Vertex
{
public:
	// Creates a Vertex 
    Vertex(int id, int x, int y, int size);

	// Destructor
    ~Vertex();

	// Returns visible shape of the vertex
    QGraphicsItem *GetShape() const;

	// Returns position of the drawn shape
    QPointF *GetPosition() const;

	// Returns the ID of the vertex
    int GetId() const;

	// Checks if the vertex is a wall
    bool IsWall() const;

	// Checks if the vertex is a start vertex
    bool IsStart() const;

	// Checks if the vertex is a goal
    bool IsGoal() const;

	// Check if a vertex has been visited
	bool WasVisited() const;

	// Gets the previously visited vertex
	Vertex *GetPrevious() const;

	// Sets the descriptive text of the vertex shape to the current vertex ID
    void SetDescription() const;

	// Sets the current vertex as a starting point
    void SetStart(bool start);

	// Sets the current vertex as the goal
    void SetGoal(bool goal);

	// Set the previously visited vertex pointer
	void SetPrevious(Vertex *vertex);

	// Sets the current vertex as visited
	void SetVisited(bool visited);

	// Sets the vertex to a wall vertex
    void SetWall();

	// Remove wall from a vertex
    void UnsetWall();

	// Highlights the vertex 
	void TracePath() const;
private:
	// Size of the vertex's shape
    int m_shapeSize;

	// Unique integer ID of the vertex
    int m_id;

	// Shape representation of the vertex
    QGraphicsRectItem *m_shape;

	// Vertex shape position
    QPointF *m_position;

	// Vertex text description
    QGraphicsTextItem *m_desc;

	// Pointer for search algorithms to find the previous vertex
	Vertex *m_previous;

	// Flags to determine status of a vertex
    bool m_wall;
    bool m_start;
    bool m_goal;
	bool m_visited;
};

