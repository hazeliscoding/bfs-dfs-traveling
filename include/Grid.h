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

#include "Node.h"
#include "PathFinder.h"

class Grid final : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Grid(QWidget *parent = nullptr);

	// Creates an array of the different possible grid sizes
    void CreateGridSizes();
public slots:
    void mousePressEvent(QMouseEvent *me) override;
protected:
	// Initializes all the UI items and arranges them in a GridLayout
    void InitUI();

	// Sets up the Grid group box
    void AddItemsToGridGroupBox(QGroupBox *groupBox);

	// Adds the shapes from the nodes to the scene
    void AddItemsToScene() const;

	// Set the start and goal nodes
    void SetStartAndGoal() const;

	// Default UI selections
    void SetDefaultSelections();

	// Traces back a path from the exit (if it exists)
	int TracePath(Node *lastNode, QStack<int> *nodeStack);

	// Draws the grid
    void Render();
private:
    // UI Objects
    QWidget *m_currentTab;
    QGridLayout *m_tabLayout;
    QGraphicsScene *m_scene;
    QComboBox *m_algoSelection;
    QComboBox *m_gridSizeSelection;

    // Buttons
    QPushButton *m_resetGridButton;
    QPushButton *m_startTravelButton;
    QPushButton *m_stopTravelButton;
    QPushButton *m_clearGridButton;

    // Grid attributes
    int m_gridSceneHeight;
    int m_gridSceneWidth;
    int m_squareSize;
    int **m_squareSizes;
    int m_nodeDescThreshold;

	// A hash list to lookup nodes by their rendered shape
    QHash<QGraphicsItem*, Node*> *m_listOfSquares;

	// A hash list to lookup nodes by their unique ID
    QHash<int, Node*> *m_listOfIds;

	// Object for traversing the grid
	PathFinder *m_pathFinder;
private slots:
	// Sets a new grid size
    void NewGridSize();

	// Starts the search algorithm
    void StartTraveling();

	// Stops the search
    void StopTraveling();

	// Resets grid but leaves walls intact
    void ResetGrid();

	// Resets grid entirely
    void ClearGrid();

	// Displays the result of the search
	void DisplayResults(Node *node);
};

