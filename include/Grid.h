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

#include "Node.h"

class Grid : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Grid(QWidget *parent = nullptr);

    void CreateGridSizes();
public slots:
    void mousePressEvent(QMouseEvent *me) override;
protected:
    void InitUI();
    void AddItemsToGridGroupBox(QGroupBox *groupBox);
    void AddItemsToScene();
    void SetStartAndGoal() const;
    void SetDefaultSelections();
    void Render();
private:
    // UI Objects
    QWidget *m_currentTab;
    QGridLayout *m_tabLayout;
    QGraphicsScene *m_scene;
    QComboBox *m_algoSelection;
    QComboBox *m_gridSizeSelection;

    // Buttons
    QPushButton *m_changeGridSizeButton;
    QPushButton *m_resetGridButton;
    QPushButton *m_startTravelButton;
    QPushButton *m_stopTravelButton;
    QPushButton *m_clearGridButton;

    // Attributes
    int m_gridSceneHeight;
    int m_gridSceneWidth;
    int m_squareSize;
    int **m_squareSizes;
    int m_nodeDescThreshold;

    // Data structures
    QHash<QGraphicsItem*, Node*> *m_listOfSquares;
    QHash<int, Node*> *m_listOfIds;
private slots:
    void NewGridSize();
    void StartTraveling();
    void StopTraveling();
    void ResetGrid();
    void ClearGrid();
};

