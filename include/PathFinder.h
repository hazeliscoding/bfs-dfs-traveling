#pragma once

#include <QTimer>
#include <QElapsedTimer>
#include <QQueue>

#include "Vertex.h"

// Tick-rate at which the algorithm runs
#define TICK 5

class PathFinder : public QObject
{
	Q_OBJECT
public:
	explicit PathFinder(QHash<int, Vertex*> *listOfIds, int rows, int cols, QObject *parent = nullptr);

	// Sets the needed values for solving traversals
	void Setup(QHash<int, Vertex*> *listOfIds, int rows, int cols);

	// Starts the BFS algorithm on the list of vertexs
	void StartBreadthFirstSearch();

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
	// Used to get vertexs by ID
	QHash<int, Vertex*> *m_hash;

	// Queue for BFS
	QQueue<Vertex*> *m_queue;

	// Timer that triggers a step BFS algorithm
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
	// Performs one step in the BFS search algorithm
	void RouteBFS();
signals:
	// Display the path/goal
	void DisplayGoal(Vertex *goal);
};