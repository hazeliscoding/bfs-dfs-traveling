#pragma once

#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QMouseEvent>
#include <QObject>
#include <QStack>
#include <QMessageBox>

#include <iostream>

#include "Vertex.h"
#include "PathFinder.h"

using SizeList = std::vector<std::pair<int, int>>;

class Graph final : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = nullptr);
public slots:
    void mousePressEvent(QMouseEvent *me) override;
protected:
	// Initializes all the UI items and arranges them in a GridLayout
    void InitUI();

	// Sets up the Graph group box
    void AddItemsToGroupBox(QGroupBox *groupBox);

	// Set the start and goal vertices
    void SetStartAndGoal() const;

	// Default UI selections
    void SetDefaultSelections();

	// Traces back a path from the exit (if it exists)
    static int TracePath(Vertex *lastVertex, QStack<int> *stack);

	// Switches UI elements on and off
	void UpdateUiState();

	// Draws the Graph
    void Render() const;
private:
    // UI Objects
    QWidget *m_currentTab;
    QGridLayout *m_tabLayout;
    QGraphicsScene *m_scene;
    QComboBox *m_algorithmSelection;
    QComboBox *m_sizeSelection;

    // Buttons
    QPushButton *m_resetGraphButton;
    QPushButton *m_startTravelButton;
    QPushButton *m_stopTravelButton;
    QPushButton *m_clearGraphButton;
	QPushButton *m_randomizeGraphButton;

    // Graph attributes
    int m_sceneHeight;
    int m_sceneWidth;
    int m_cellSize;
    int m_vertexDescThreshold;
	SizeList m_sizeList;

	// A hash list to lookup vertices by their rendered shape
	VertexHashShapeList *m_vertices;

	// A hash list to lookup vertices by their unique ID
	VertexHashIDList *m_vertexIdList;

	// Object for traversing the Graph
	PathFinder *m_pathFinder;

	// Flag to prevent wall set/un-setting during traversals
	bool m_currentlyTraveling;
private slots:
	// Sets a new Graph size
    void NewSize();

	// Starts the search algorithm
    void StartTraveling();

	// Stops the search
    void StopTraveling();

	// Resets Graph but leaves walls intact
    void Reset() const;

	// Resets Graph entirely
    void Clear() const;

	// Place random walls on Graph
	void Randomize() const;

	// Displays the result of the search
	void DisplayResults(Vertex *vertex);
};

