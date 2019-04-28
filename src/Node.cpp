#include "Node.h"

Node::Node(const int id, const int x, const int y, const int size)
	: m_id(id)
	, m_previous(nullptr)
	, m_wall(false)
	, m_start(false)
	, m_goal(false)
	, m_visited(false)
{
	// Create visual representation of the node
	this->m_shape = new QGraphicsRectItem(x, y, size, size);

	// Set position
	this->m_position = new QPointF(x, y);

	// Description of the node
	this->m_desc = new QGraphicsTextItem("", this->m_shape);
	this->m_desc->setPos(this->m_position->x(), this->m_position->y());
}

Node::~Node()
{
    delete this->m_position;
}

QGraphicsItem *Node::GetShape() const
{
    return this->m_shape;
}

QPointF *Node::GetPosition() const
{
    return this->m_position;
}

int Node::GetId() const
{
    return this->m_id;
}

bool Node::IsWall() const
{
    return this->m_wall;
}

bool Node::IsStart() const
{
    return this->m_start;
}

bool Node::IsGoal() const
{
    return this->m_goal;
}

bool Node::WasVisited() const
{
	return this->m_visited;
}

Node* Node::GetPreviousNode() const
{
	return this->m_previous;
}

void Node::SetDescription() const
{
    this->m_desc->setPlainText(QString::number(this->m_id));
}

void Node::SetStart(const bool start)
{
    this->m_start = start;
    if (start)
        this->m_shape->setBrush(QBrush(QColor(0, 255, 72))); // Bright green
}

void Node::SetGoal(const bool goal)
{
    this->m_goal = goal;
    if (goal)
        this->m_shape->setBrush(QBrush(Qt::red));
}

void Node::SetActive(bool active)
{
	if (active)
	{
		this->m_shape->setBrush(QBrush(Qt::yellow));
	}
	else
	{
		this->m_shape->setBrush(QBrush(Qt::white));
	}
}

void Node::SetPreviousNode(Node *node)
{
	this->m_previous = node;
}

void Node::SetVisited(bool visited)
{
	this->m_visited = visited;

	if (visited)
	{
		this->m_shape->setBrush(QBrush(Qt::cyan)); // Dark green
	}
	else
	{
		this->m_shape->setBrush(QBrush(Qt::white));
	}
}

void Node::SetWall()
{
    if (this->m_start || this->m_goal)
        return;

    this->m_wall = true;
    this->m_shape->setBrush(QBrush(Qt::gray));
}

void Node::UnsetWall()
{
    this->m_wall = false;
    this->m_shape->setBrush(QBrush(Qt::white));
}

void Node::TracePath()
{
	this->m_shape->setBrush(QBrush(Qt::yellow));
}

