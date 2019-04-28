#pragma once

#include <QTimer>
#include <QElapsedTimer>
#include <QQueue>

#include "Node.h"

#define TICK 50

class PathFinder : public QObject
{
	Q_OBJECT
public:
	explicit PathFinder(QHash<int, Node*> *listOfIds, int rows, int cols, QObject *parent = nullptr);
	void Setup(QHash<int, Node*> *listOfIds, int rows, int cols);
	void StartBreadthFirstSearch();
protected:
	QList<Node*> *GetNeighborNodes(int id);
	void Stop(Node *node);
private:
	QHash<int, Node*> *m_hash;
	QQueue<Node*> *m_queue;
	QTimer *m_bfsTick;
	QElapsedTimer *m_elapsedTimer;
	quint64 m_timeElapsed;

	int m_rows;
	int m_cols;
	int m_tickInterval;
private slots:
	void BreadthFirstSearchStep();
signals:
	void DisplayGoal(Node *goal);
};