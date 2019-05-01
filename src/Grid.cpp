#include "Grid.h"

Grid::Grid(QWidget *parent)
	: QGraphicsView(parent), m_squareSizes(nullptr)
{
	this->m_currentTab = parent;

	// Default size of the grid canvas
	this->m_gridSceneHeight = 600;
	this->m_gridSceneWidth = 750;

	// Load selectable grid sizes
	CreateGridSizes();

	// Default values
	this->m_squareSize = this->m_squareSizes[1][0];
	this->m_nodeDescThreshold = this->m_squareSizes[1][2];

	// Data structures which hold pointers to all the nodes
	this->m_listOfSquares = new QHash<QGraphicsItem*, Node*>();
	this->m_listOfIds = new QHash<int, Node*>();

	InitUI();
	SetDefaultSelections();
	Render();

	// Initialize pathfinder
	auto cols = this->m_gridSceneWidth / this->m_squareSize;
	auto rows = this->m_gridSceneHeight / this->m_squareSize;
	this->m_pathFinder = new PathFinder(this->m_listOfIds, rows, cols);
	connect(this->m_pathFinder, SIGNAL(DisplayGoal(Node*)), this, SLOT(DisplayResults(Node*)));
}

void Grid::CreateGridSizes()
{
    // Row 1: Visible number of squares
    // Row 2: Size of squares
    this->m_squareSizes = new int*[2];
    for (auto i = 0; i < 2; i++)
        this->m_squareSizes[i] = new int[6];

	this->m_squareSizes[0][0] = 20;
	this->m_squareSizes[0][1] = 80;
	this->m_squareSizes[0][2] = 500;
	this->m_squareSizes[0][3] = 2000;

	this->m_squareSizes[1][0] = 150;
	this->m_squareSizes[1][1] = 75;
	this->m_squareSizes[1][2] = 30;
	this->m_squareSizes[1][3] = 15;
}

void Grid::mousePressEvent(QMouseEvent *me)
{
	const auto current = itemAt(me->pos());

    if (current == nullptr)
        return;

    auto selectedNode = this->m_listOfSquares->value(current);

    if (selectedNode == nullptr)
        return;

    // Set and unset walls at the clicked node
    if (me->button() == Qt::LeftButton)
    {
        if (selectedNode->IsWall())
        {
            selectedNode->UnsetWall();
        }
        else
        {
            selectedNode->SetWall();
        }
    }
}

void Grid::InitUI()
{
	auto gridLayout = new QGridLayout();
    this->m_currentTab->setLayout(gridLayout);

    // Graphics view for squares
    setGeometry(0, 0, this->m_gridSceneWidth, this->m_gridSceneHeight);
    this->m_scene = new QGraphicsScene();
    setScene(this->m_scene);
    gridLayout->addWidget(this, 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignTop);

    // Spacers
	const auto spacer = new QSpacerItem(10, 0);
    gridLayout->addItem(spacer, 0, 1);

    // Control Boxes
	const auto gridGroupBox = new QGroupBox("Grid Configuration", this);
    gridLayout->addWidget(gridGroupBox, 0, 2);
    AddItemsToGridGroupBox(gridGroupBox);
}

void Grid::AddItemsToGridGroupBox(QGroupBox *groupBox)
{
    // Setting layouts
    auto controlLayout = new QFormLayout();
    groupBox->setLayout(controlLayout);

    const auto algoDescription = new QLabel("Algorithm");
    this->m_algoSelection = new QComboBox();
    this->m_algoSelection->addItem("Depth-First Search");
    this->m_algoSelection->addItem("Breadth-First Search");
    controlLayout->addRow(algoDescription, this->m_algoSelection);

    // Display the possible sizes of the grid to user
    const auto gridSizeDesc = new QLabel("Grid Size");
    this->m_gridSizeSelection = new QComboBox();
	for (auto i = 0; i < 4; i++)
	{
		this->m_gridSizeSelection->addItem(QString::number(this->m_squareSizes[0][i]));
	}
    controlLayout->addRow(gridSizeDesc, this->m_gridSizeSelection);

    this->m_resetGridButton = new QPushButton("Reset Grid");
    controlLayout->addRow(this->m_resetGridButton);

    this->m_clearGridButton = new QPushButton("Clear Grid");
    controlLayout->addRow(this->m_clearGridButton);

	this->m_randomizeGridButton = new QPushButton("Randomize Grid");
	controlLayout->addRow(this->m_randomizeGridButton);

    this->m_startTravelButton = new QPushButton("Start Traveling");
    controlLayout->addRow(this->m_startTravelButton);

    this->m_stopTravelButton = new QPushButton("Stop Traveling");
    controlLayout->addRow(this->m_stopTravelButton);
	this->m_stopTravelButton->setVisible(false);

    // Connect UI objects to slots
	connect(this->m_gridSizeSelection, SIGNAL(activated(int)), this, SLOT(NewGridSize()));
    connect(this->m_startTravelButton, SIGNAL(clicked()), this, SLOT(StartTraveling()));
    connect(this->m_stopTravelButton, SIGNAL(clicked()), this, SLOT(StopTraveling()));
    connect(this->m_resetGridButton, SIGNAL(clicked()), this, SLOT(ResetGrid()));
    connect(this->m_clearGridButton, SIGNAL(clicked()), this, SLOT(ClearGrid()));
	connect(this->m_randomizeGridButton, SIGNAL(clicked()), this, SLOT(RandomizeGrid()));
}

void Grid::AddItemsToScene() const
{
	const auto cols = this->m_gridSceneWidth / this->m_squareSize;
    const auto rows = this->m_gridSceneHeight / this->m_squareSize;
    auto i = 0;
    auto j = 0;
    auto idCount = 0;

    // Traverse rows/cols
    while (i <  rows)
    {
        while (j < cols)
        {
            // Get a new node
            auto node = new Node(idCount, j * this->m_squareSize, i * this->m_squareSize, this->m_squareSize);
            this->m_scene->addItem(node->GetShape());

            // Insert nodes into hash tables
            this->m_listOfSquares->insert(node->GetShape(), node);
            this->m_listOfIds->insert(idCount, node);

            // Set node descriptions for the smaller sizes
            if (this->m_squareSize > this->m_nodeDescThreshold)
                node->SetDescription();

            j++;
            idCount++;
        }
        j = 0;
        i++;
    }
    SetStartAndGoal();
}

void Grid::SetStartAndGoal() const
{
    this->m_listOfIds->value(0)->SetStart(true);
    this->m_listOfIds->value(this->m_listOfIds->size() - 1)->SetGoal(true);
}

void Grid::SetDefaultSelections()
{
    // Set default grid size
    this->m_gridSizeSelection->setCurrentIndex(1);
    this->m_squareSize = this->m_squareSizes[1][this->m_gridSizeSelection->currentIndex()];

    // Set default algorithm
    this->m_algoSelection->setCurrentIndex(1);
}

int Grid::TracePath(Node* lastNode, QStack<int>* nodeStack)
{
	auto count = 0;
	while (lastNode != nullptr)
	{
		lastNode->TracePath();
		nodeStack->push(lastNode->GetId());
		lastNode = lastNode->GetPreviousNode();
		count++;
	}
	return count;
}

void Grid::UpdateUiState()
{
	this->m_startTravelButton->setEnabled(!this->m_startTravelButton->isEnabled());
	this->m_resetGridButton->setEnabled(!this->m_resetGridButton->isEnabled());
	this->m_gridSizeSelection->setEnabled(!this->m_gridSizeSelection->isEnabled());
	this->m_algoSelection->setEnabled(!this->m_algoSelection->isEnabled());
	this->m_clearGridButton->setEnabled(!this->m_clearGridButton->isEnabled());
	this->m_randomizeGridButton->setEnabled(!this->m_randomizeGridButton->isEnabled());
}

void Grid::Render()
{
    AddItemsToScene();
}

void Grid::NewGridSize()
{
    if (this->m_squareSize == this->m_squareSizes[1][this->m_gridSizeSelection->currentIndex()])
        return;

    this->m_squareSize = this->m_squareSizes[1][this->m_gridSizeSelection->currentIndex()];
    this->m_scene->clear();

    for (auto &node : *this->m_listOfSquares)
        delete node;

    this->m_listOfSquares->clear();
    this->m_listOfIds->clear();
    AddItemsToScene();
    this->m_startTravelButton->setEnabled(true);
}

void Grid::StartTraveling()
{
	UpdateUiState();
	this->m_startTravelButton->setVisible(false);
	this->m_stopTravelButton->setVisible(true);

	const auto cols = this->m_gridSceneWidth / this->m_squareSize;
	const auto rows = this->m_gridSceneHeight / this->m_squareSize;

	this->m_pathFinder->Setup(this->m_listOfIds, rows, cols);

	if (this->m_algoSelection->currentText() == "Depth-First Search")
	{
		qDebug() << "DFS started.\n";
	}
	else if (this->m_algoSelection->currentText() == "Breadth-First Search")
	{
		this->m_pathFinder->StartBreadthFirstSearch();
	}
}

void Grid::StopTraveling()
{
    // TODO: Stop traveling in mid-search
}

void Grid::ResetGrid()
{
    for (auto& node : *this->m_listOfSquares)
    {
	    if (!node->IsWall())
	    {
			node->UnsetWall();
			node->SetStart(false);
			node->SetVisited(false);
			node->SetGoal(false);

			if (this->m_squareSize > this->m_nodeDescThreshold)
				node->SetDescription();
	    }
    }

	// Reset start/goal
	SetStartAndGoal();

	this->m_startTravelButton->setEnabled(true);
}

void Grid::ClearGrid()
{
    for (auto& node : *this->m_listOfSquares)
    {
		node->UnsetWall();
		node->SetStart(false);
		node->SetVisited(false);
		node->SetGoal(false);

		if (this->m_squareSize > this->m_nodeDescThreshold)
			node->SetDescription();
    }

	// Reset start/goal
	SetStartAndGoal();

	this->m_startTravelButton->setEnabled(true);
}

void Grid::RandomizeGrid()
{
	ClearGrid();
	for (auto node : *this->m_listOfSquares)
	{
		if (!node->IsGoal() || !node->IsStart())
		{
			const auto heuristic = qrand() % 3; // Decides if it's a wall or not
			if (heuristic >= 2)
			{
				node->SetWall();
			}
		}
	}
}

void Grid::DisplayResults(Node* node)
{
	// Trace the path
	if (node != nullptr)
	{
		const auto path = new QStack<int>();
		const auto pathLength = TracePath(node, path);

#ifdef QT_DEBUG
		while (!path->isEmpty())
		{
			qDebug() << QString::number(path->pop());
		}
		qDebug() << "Length of the path: " + QString::number(pathLength);
		qDebug() << "Seconds elapsed: " + QString::number(m_pathFinder->GetElapsedTime() / 1000.0, 'f', 2);
#else
		QMessageBox::information(this, "Path Length",
			"Length of the path: " + QString::number(pathLength)
			+ "\nSeconds elapsed: " + QString::number(m_pathFinder->GetElapsedTime() / 1000.0, 'f', 2));
#endif
	}
	else
	{
#ifdef QT_DEBUG
		qDebug() << "No path found!";
#else
		QMessageBox::information(this, "NULL", "No path found!");
#endif
	}

	// Disable searching until grid is reset
	UpdateUiState();
	this->m_startTravelButton->setEnabled(false);
	this->m_startTravelButton->setVisible(true);
	this->m_stopTravelButton->setVisible(false);
}
