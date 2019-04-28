#include "PathFinder.h"

PathFinder::PathFinder(QHash<int, Node*>* listOfIds, const int rows, const int cols, QObject* parent)
	: m_hash(listOfIds)
	, m_queue(nullptr)
	, m_rows(rows)
	, m_cols(cols)
{
	// Init timers
	this->m_bfsTick = new QTimer(this);
	this->m_elapsedTimer = new QElapsedTimer();
	this->m_timeElapsed = 0;

	// Connect algorithm steps with timers
	connect(this->m_bfsTick, SIGNAL(timeout()), this, SLOT(BreadthFirstSearchStep()));
}

void PathFinder::Setup(QHash<int, Node*>* listOfIds, int rows, int cols)
{
	this->m_rows = rows;
	this->m_cols = cols;
	this->m_hash = listOfIds;
}

void PathFinder::StartBreadthFirstSearch()
{
	// Setup queue and timer to use for BFS
	this->m_queue = new QQueue<Node*>();
	this->m_elapsedTimer->restart();	

	// Starting point
	Node *start = this->m_hash->value(0);
	this->m_queue->enqueue(start);

	// On each tick, take one node off the queue and search
	this->m_bfsTick->blockSignals(false);
	this->m_bfsTick->start(TICK);
}

QList<Node*>* PathFinder::GetNeighborNodes(int id)
{
	auto *neighborNodes = new QList<Node*>();
	const auto numNodes = this->m_rows * this->m_cols;

	Node *adjacentNode;

	// South
	if ((id) < (numNodes - this->m_cols))
	{
		adjacentNode = this->m_hash->value(id + this->m_cols);
		if (!adjacentNode->IsWall() && !adjacentNode->WasVisited())
		{
			neighborNodes->append(adjacentNode);
		}
	}

	// North
	if (id > this->m_cols)
	{
		adjacentNode = this->m_hash->value(id - this->m_cols);
		if (!adjacentNode->IsWall() && !adjacentNode->WasVisited())
		{
			neighborNodes->append(adjacentNode);
		}
	}

	// East
	if (((id + 1) % this->m_cols) != 0)
	{
		adjacentNode = this->m_hash->value(id + 1);
		if (!adjacentNode->IsWall() && (id + 1) <= numNodes && !adjacentNode->WasVisited())
		{
			neighborNodes->append(adjacentNode);
		}
	}

	// West
	if (id != 0 && ((id) % this->m_cols) != 0)
	{
		adjacentNode = this->m_hash->value(id - 1);
		if (!adjacentNode->IsWall() && !adjacentNode->WasVisited())
		{
			neighborNodes->append(adjacentNode);
		}
	}
	return neighborNodes;
}

void PathFinder::Stop(Node* node)
{
	// Stop ALL timers
	this->m_timeElapsed = this->m_elapsedTimer->elapsed();
	this->m_elapsedTimer->invalidate();

	this->m_bfsTick->blockSignals(true);
	this->m_bfsTick->stop();

	// Clear containers
	if (this->m_queue != nullptr)
		this->m_queue->clear();

	// Display the path
	emit DisplayGoal(node);
}

void PathFinder::BreadthFirstSearchStep()
{
	Node *nextNode;
	auto validNodeCounter = 0;
	auto goalFound = false;

	/*
	 * Breadth-First Search algorithm
	 * 
	 * Dequeue a node from the queue.
	 * Get adjacent nodes and iterate over them at random and check if they're goals.
	 * It not, enqueue them on the queue.
	 */

	if (!this->m_queue->isEmpty())
	{
		auto currentNode = this->m_queue->dequeue();

		if (currentNode->WasVisited())
			return;

		currentNode->SetActive(true);
		const auto currentId = currentNode->GetId();
		auto neighborNodes = GetNeighborNodes(currentId);

		while (!neighborNodes->empty())
		{
			nextNode = neighborNodes->takeFirst();

			// Ignore walls or nodes that are already visited
			if (nextNode->IsWall() || nextNode->WasVisited())
				continue;

			validNodeCounter++;
			nextNode->SetPreviousNode(currentNode);
			this->m_queue->enqueue(nextNode);

			if (nextNode->IsGoal())
			{
				goalFound = true;
				break;
			}
		}
		currentNode->SetVisited(true);

		if (goalFound)
			Stop(nextNode); // Goal found
	}
	else
	{
		Stop(nullptr); // Goal NOT found
		return;
	}

	// No node has been updated in current tick
	if (validNodeCounter == 0)
	{
		validNodeCounter = 0;
		BreadthFirstSearchStep();
	}
}
