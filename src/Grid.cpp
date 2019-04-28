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
	this->m_pathFinder = new PathFinder(this->m_listOfIds, rows, cols, 10);
	connect(this->m_pathFinder, SIGNAL(DisplayGoal(Node*)), this, SLOT(DisplayResults(Node*)));
}

void Grid::CreateGridSizes()
{
    // Row 1: Visible number of squares
    // Row 2: Size of squares
    this->m_squareSizes = new int*[2];
    for (auto i = 0; i < 2; i++)
        this->m_squareSizes[i] = new int[3];

    m_squareSizes[0][0] = 20;
    m_squareSizes[0][1] = 80;
    m_squareSizes[0][2] = 100;

    m_squareSizes[1][0] = 150;
    m_squareSizes[1][1] = 75;
    m_squareSizes[1][2] = 50;
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
	const auto gridGroupBox = new QGroupBox("Grid", this);
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
    this->m_gridSizeSelection->addItem(QString::number(this->m_squareSizes[0][0]));
    this->m_gridSizeSelection->addItem(QString::number(this->m_squareSizes[0][1]));
    this->m_gridSizeSelection->addItem(QString::number(this->m_squareSizes[0][2]));
    controlLayout->addRow(gridSizeDesc, this->m_gridSizeSelection);

    this->m_changeGridSizeButton = new QPushButton("Change Grid Size");
    controlLayout->addRow(this->m_changeGridSizeButton);

    this->m_resetGridButton = new QPushButton("Reset Grid");
    controlLayout->addRow(this->m_resetGridButton);

    this->m_clearGridButton = new QPushButton("Clear Grid");
    controlLayout->addRow(this->m_clearGridButton);

    this->m_startTravelButton = new QPushButton("Start Traveling");
    controlLayout->addRow(this->m_startTravelButton);

    this->m_stopTravelButton = new QPushButton("Stop Traveling");
    controlLayout->addRow(this->m_stopTravelButton);

    // Connect buttons to slots
    connect(this->m_changeGridSizeButton, SIGNAL(clicked()), this, SLOT(NewGridSize()));
    connect(this->m_startTravelButton, SIGNAL(clicked()), this, SLOT(StartTraveling()));
    connect(this->m_stopTravelButton, SIGNAL(clicked()), this, SLOT(StopTraveling()));
    connect(this->m_resetGridButton, SIGNAL(clicked()), this, SLOT(ResetGrid()));
    connect(this->m_clearGridButton, SIGNAL(clicked()), this, SLOT(ClearGrid()));
}

void Grid::AddItemsToScene()
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
    this->m_algoSelection->setCurrentIndex(0);
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

    for (auto &item : *this->m_listOfSquares)
        delete item;

    this->m_listOfSquares->clear();
    this->m_listOfIds->clear();
    AddItemsToScene();
    this->m_startTravelButton->setEnabled(true);
}

void Grid::StartTraveling()
{
	qDebug() << "Traveling...\n";

	const auto cols = this->m_gridSceneWidth / this->m_squareSize;
	const auto rows = this->m_gridSceneHeight / this->m_squareSize;

	this->m_pathFinder->Setup(this->m_listOfIds, rows, cols, 10);

	if (this->m_algoSelection->currentText() == "Depth-First Search")
	{
		qDebug() << "DFS started.\n";
	}
	else if (this->m_algoSelection->currentText() == "Breadth-First Search")
	{
		qDebug() << "BFS started.\n";
		this->m_pathFinder->StartBreadthFirstSearch();
	}
	else
	{
		
	}
}

void Grid::StopTraveling()
{
    // TODO: Stop traveling in mid-search
}

void Grid::ResetGrid()
{
    // TODO: Reset the search
}

void Grid::ClearGrid()
{
    // TODO: Implement clearing the whole grid
}
