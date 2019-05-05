#include "PathFinder.h"

PathFinder::PathFinder(QHash<int, Vertex*>* listOfIds, const int rows, const int cols, QObject* parent)
	: m_hash(listOfIds)
	, m_queue(nullptr)
	, m_stack(nullptr)
	, m_rows(rows)
	, m_cols(cols)
	, m_interrupted(false)
{
	// Init timers
	this->m_bfsTick = new QTimer(this); 
	this->m_dfsTick = new QTimer(this);
	this->m_timer = new QElapsedTimer();
	this->m_timeElapsed = 0;

	// Connect algorithm steps with timers
	connect(this->m_bfsTick, SIGNAL(timeout()), this, SLOT(RouteBFS()));
	connect(this->m_dfsTick, SIGNAL(timeout()), this, SLOT(RouteDFS()));
}

void PathFinder::Setup(QHash<int, Vertex*>* listOfIds, const int rows, const int cols)
{
	this->m_rows = rows;
	this->m_cols = cols;
	this->m_hash = listOfIds;
}

void PathFinder::StartBreadthFirstSearch()
{
	// Setup queue and timer to use for BFS
	this->m_queue = new QQueue<Vertex*>();
	this->m_timer->restart();

	// Starting point
	Vertex *start = this->m_hash->value(0);
	this->m_queue->enqueue(start);

	// On each tick, take one vertex off the queue and search
	this->m_bfsTick->blockSignals(false);
	this->m_bfsTick->start(TICK_RATE);
}

void PathFinder::StartDepthFirstSearch()
{
	// Setup stack and timer to use for DFS
	this->m_stack = new QStack<Vertex*>();
	this->m_timer->restart();

	// Starting point
	Vertex *start = this->m_hash->value(0);
	this->m_stack->push(start);

	// On each tick, take one vertex off the stack and search
	this->m_dfsTick->blockSignals(false);
	this->m_dfsTick->start(TICK_RATE);
}

quint64 PathFinder::GetElapsedTime() const
{
	return this->m_timeElapsed;
}

void PathFinder::TriggerInterrupt()
{
	this->m_timeElapsed = this->m_timer->elapsed();
	this->m_timer->invalidate();
	m_interrupted = true;
}

QList<Vertex*>* PathFinder::GetNeighbors(const int id) const
{
	auto neighbors = new QList<Vertex*>();
	const auto numVertices = this->m_rows * this->m_cols;

	Vertex *adjacentVertex;

	// South
	if ((id) < (numVertices - this->m_cols))
	{
		adjacentVertex = this->m_hash->value(id + this->m_cols);
		if (!adjacentVertex->IsWall() && !adjacentVertex->WasVisited())
		{
			neighbors->append(adjacentVertex);
		}
	}

	// North
	if (id > this->m_cols)
	{
		adjacentVertex = this->m_hash->value(id - this->m_cols);
		if (!adjacentVertex->IsWall() && !adjacentVertex->WasVisited())
		{
			neighbors->append(adjacentVertex);
		}
	}

	// East
	if (((id + 1) % this->m_cols) != 0)
	{
		adjacentVertex = this->m_hash->value(id + 1);
		if (!adjacentVertex->IsWall() && (id + 1) <= numVertices && !adjacentVertex->WasVisited())
		{
			neighbors->append(adjacentVertex);
		}
	}

	// West
	if (id != 0 && ((id) % this->m_cols) != 0)
	{
		adjacentVertex = this->m_hash->value(id - 1);
		if (!adjacentVertex->IsWall() && !adjacentVertex->WasVisited())
		{
			neighbors->append(adjacentVertex);
		}
	}
	return neighbors;
}

void PathFinder::Stop(Vertex* vertex)
{
	// Stop ALL timers
	this->m_timeElapsed = this->m_timer->elapsed();
	this->m_timer->invalidate();

	this->m_bfsTick->blockSignals(true);
	this->m_bfsTick->stop();

	this->m_dfsTick->blockSignals(true);
	this->m_dfsTick->stop();

	// Clear containers
	if (this->m_queue != nullptr)
		this->m_queue->clear();

	if (this->m_stack != nullptr)
		this->m_stack->clear();

	// Display the path
	emit DisplayGoal(vertex);
}

void PathFinder::RouteBFS()
{
	// Check if this algorithm has been interrupted while running
	if (this->m_interrupted)
	{
		this->m_bfsTick->blockSignals(false);
		this->m_bfsTick->stop();
		this->m_interrupted = false;
		Stop(nullptr); // Interrupt the search, pass in nullptr as the goal hasn't been found
		return;
	}

	Vertex *next;
	auto validVertexCounter = 0;
	auto goalFound = false;

	/*
	 * Breadth-First Search algorithm
	 * 
	 * Dequeue a vertex from the queue.
	 * Get adjacent nodes and iterate over them at random and check if they're goals.
	 * It not, enqueue them on the queue.
	 */

	if (!this->m_queue->isEmpty())
	{
		auto current = this->m_queue->dequeue();

		if (current->WasVisited())
			return;

		const auto currentId = current->GetId();
		auto neighbors = GetNeighbors(currentId);

		while (!neighbors->empty())
		{
			next = neighbors->takeFirst();

			// Ignore walls or nodes that are already visited
			if (next->IsWall() || next->WasVisited())
				continue;

			validVertexCounter++;
			next->SetPrevious(current);
			this->m_queue->enqueue(next);

			if (next->IsGoal())
			{
				goalFound = true;
				break;
			}
		}
		current->SetVisited(true);

		if (goalFound)
			Stop(next); // Goal found
	}
	else
	{
		Stop(nullptr); // Goal NOT found
		return;
	}

	// No vertex has been updated in current tick
	if (validVertexCounter == 0)
	{
		validVertexCounter = 0;
		RouteBFS();
	}
}

void PathFinder::RouteDFS()
{
	// Check if this algorithm has been interrupted while running
	if (this->m_interrupted)
	{
		this->m_dfsTick->blockSignals(false);
		this->m_dfsTick->stop();
		this->m_interrupted = false;
		Stop(nullptr); // Interrupt the search, pass in nullptr as the goal hasn't been found
		return;
	}

	Vertex *next;
	auto validVertexCounter = 0;
	auto goalFound = false;

	/*
	 * Depth-First Search algorithm
	 *
	 * Pop a node off of the stack.
	 * Get adjacent nodes and iterate over them and check if they're goals.
	 * It not, push them onto the stack.
	 */

	if (!this->m_stack->isEmpty())
	{
		auto current = this->m_stack->pop();

		const auto currentId = current->GetId();
		auto neighbors = GetNeighbors(currentId);

		while (!neighbors->isEmpty())
		{
			for (auto i = 0; i < neighbors->size(); i++)
				next = neighbors->takeAt(i);

			// Ignore walls and visited nodes
			if (next->IsWall() || next->WasVisited())
				continue;

			validVertexCounter++;
			next->SetPrevious(current);
			this->m_stack->push(next);

			if (next->IsGoal())
			{
				goalFound = true;
				break;
			}
		}
		current->SetVisited(true);

		if (goalFound)
			Stop(next); // Goal found
	}
	else
	{
		Stop(nullptr); // Goal NOT found
		return;
	}

	// No vertex has been updated in current tick
	if (validVertexCounter == 0)
	{
		validVertexCounter = 0;
		this->m_dfsTick->stop();
		RouteDFS();
		this->m_dfsTick->start(TICK_RATE);
	}
}
