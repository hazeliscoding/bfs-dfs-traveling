#include "Vertex.h"

Vertex::Vertex(const int id, const int x, const int y, const int size)
	: m_id(id)
	, m_previous(nullptr)
	, m_wall(false)
	, m_start(false)
	, m_goal(false)
	, m_visited(false)
{
	// Create visual representation of the vertex
	this->m_shape = new QGraphicsRectItem(x, y, size, size);

	// Set position
	this->m_position = new QPointF(x, y);

	// Description of the vertex
	this->m_desc = new QGraphicsTextItem("", this->m_shape);
	this->m_desc->setPos(this->m_position->x(), this->m_position->y());
}

Vertex::~Vertex()
{
    delete this->m_position;
}

QGraphicsItem *Vertex::GetShape() const
{
    return this->m_shape;
}

QPointF *Vertex::GetPosition() const
{
    return this->m_position;
}

int Vertex::GetId() const
{
    return this->m_id;
}

bool Vertex::IsWall() const
{
    return this->m_wall;
}

bool Vertex::IsStart() const
{
    return this->m_start;
}

bool Vertex::IsGoal() const
{
    return this->m_goal;
}

bool Vertex::WasVisited() const
{
	return this->m_visited;
}

Vertex* Vertex::GetPrevious() const
{
	return this->m_previous;
}

void Vertex::SetDescription() const
{
    this->m_desc->setPlainText(QString::number(this->m_id));
}

void Vertex::SetStart(const bool start)
{
    this->m_start = start;
    if (start)
        this->m_shape->setBrush(QBrush(Qt::green)); 
}

void Vertex::SetGoal(const bool goal)
{
    this->m_goal = goal;
    if (goal)
        this->m_shape->setBrush(QBrush(Qt::red));
}

void Vertex::SetPrevious(Vertex *vertex)
{
	this->m_previous = vertex;
}

void Vertex::SetVisited(bool visited)
{
	this->m_visited = visited;

	if (visited)
	{
		this->m_shape->setBrush(QBrush(QColor(135, 206, 250))); // Sky blue
	}
	else
	{
		this->m_shape->setBrush(QBrush(Qt::white));
	}
}

void Vertex::SetWall()
{
    if (this->m_start || this->m_goal)
        return;

    this->m_wall = true;
    this->m_shape->setBrush(QBrush(Qt::gray));
}

void Vertex::UnsetWall()
{
    this->m_wall = false;
    this->m_shape->setBrush(QBrush(Qt::white));
}

void Vertex::TracePath() const
{
	this->m_shape->setBrush(QBrush(Qt::yellow));
}

