#pragma once

#include <QTimer>
#include <QElapsedTimer>
#include <QQueue>
#include <QStack>

#include "Vertex.h"

// Tick-rate at which the algorithm runs
#define TICK_RATE 1

class PathFinder : public QObject
{
	Q_OBJECT
public:
	explicit PathFinder(QHash<int, Vertex*> *listOfIds, int rows, int cols, QObject *parent = nullptr);

	// Sets the needed values for solving traversals
	void Setup(VertexHashIDList *listOfIds, int rows, int cols);

	// Starts the BFS algorithm on the list of vertices
	void StartBreadthFirstSearch();

	// Starts the DFS algorithm on the list of vertices
	void StartDepthFirstSearch();

	// Gets the time elapsed during search
	quint64 GetElapsedTime() const;

	// Stops the algorithm, triggered from the UI
	void TriggerInterrupt();
protected:
	// Gets the adjacent vertexs which have not been visited
	QList<Vertex*> *GetNeighbors(int id) const;

	// Stops a algorithm
	void Stop(Vertex *vertex);
private:
	// Used to get vertices by ID
	VertexHashIDList *m_hash;

	// Queue for BFS
	QQueue<Vertex*> *m_queue;

	// Stack for DFS
	QStack<Vertex*> *m_stack;

	// Timers that triggers a step DFS, BFS algorithm
	QTimer *m_dfsTick;
	QTimer *m_bfsTick;

	// Measures elapsed time when performing an algorithm
	QElapsedTimer *m_timer;

	// Time elapsed during an algorithm
	quint64 m_timeElapsed;

	// N-rows and N-columns to use in the algorithm
	int m_rows;
	int m_cols;

	// Flag to interrupt performing an algorithm
	bool m_interrupted;
private slots:
	// These functions perform one step in the BFS, DFS search algorithms
	void RouteBFS();
	void RouteDFS();
signals:
	// Display the path/goal
	void DisplayGoal(Vertex *goal);
};