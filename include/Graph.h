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

class Graph final : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = nullptr);

	// Creates an array of the different possible Graph sizes of N x N
    void CreateSizes();
public slots:
    void mousePressEvent(QMouseEvent *me) override;
protected:
	// Initializes all the UI items and arranges them in a GridLayout
    void InitUI();

	// Sets up the Graph group box
    void AddItemsToGroupBox(QGroupBox *groupBox);

	// Adds the shapes from the vertices to the scene
    void AddItemsToScene() const;

	// Set the start and goal vertices
    void SetStartAndGoal() const;

	// Default UI selections
    void SetDefaultSelections();

	// Traces back a path from the exit (if it exists)
	int TracePath(Vertex *lastVertex, QStack<int> *stack);

	// Switches UI elements on and off
	void UpdateUiState();

	// Draws the Graph
    void Render();
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
    int **m_cellSizes;
    int m_vertexDescThreshold;

	// A hash list to lookup vertices by their rendered shape
    QHash<QGraphicsItem*, Vertex*> *m_cellList;

	// A hash list to lookup vertices by their unique ID
    QHash<int, Vertex*> *m_idList;

	// Object for traversing the Graph
	PathFinder *m_pathFinder;
private slots:
	// Sets a new Graph size
    void NewSize();

	// Starts the search algorithm
    void StartTraveling();

	// Stops the search
    void StopTraveling();

	// Resets Graph but leaves walls intact
    void Reset();

	// Resets Graph entirely
    void Clear();

	// Place random walls on Graph
	void Randomize();

	// Displays the result of the search
	void DisplayResults(Vertex *vertex);
};

