#include "Graph.h"

Graph::Graph(QWidget *parent)
	: QGraphicsView(parent), m_cellSizes(nullptr)
{
	this->m_currentTab = parent;

	// Default size of the Graph canvas
	this->m_sceneHeight = 600;
	this->m_sceneWidth = 750;

	// Load selectable Graph sizes
	CreateSizes();

	// Default values
	this->m_cellSize = this->m_cellSizes[1][0];
	this->m_vertexDescThreshold = this->m_cellSizes[1][2];

	// Data structures which hold pointers to all the vertices
	this->m_cellList = new QHash<QGraphicsItem*, Vertex*>();
	this->m_idList = new QHash<int, Vertex*>();

	InitUI();
	SetDefaultSelections();
	Render();

	// Initialize pathfinder
	const auto cols = this->m_sceneWidth / this->m_cellSize;
	const auto rows = this->m_sceneHeight / this->m_cellSize;
	this->m_pathFinder = new PathFinder(this->m_idList, rows, cols);
	connect(this->m_pathFinder, SIGNAL(DisplayGoal(Vertex*)), this, SLOT(DisplayResults(Vertex*)));
}

void Graph::CreateSizes()
{
    // Row 1: Visible number of squares
    // Row 2: Size of squares
    this->m_cellSizes = new int*[2];
    for (auto i = 0; i < 2; i++)
        this->m_cellSizes[i] = new int[6];

	this->m_cellSizes[0][0] = 20;
	this->m_cellSizes[0][1] = 80;
	this->m_cellSizes[0][2] = 500;
	this->m_cellSizes[0][3] = 2000;
	this->m_cellSizes[0][4] = 4500;
	this->m_cellSizes[0][5] = 18000;

	this->m_cellSizes[1][0] = 150;
	this->m_cellSizes[1][1] = 75;
	this->m_cellSizes[1][2] = 30;
	this->m_cellSizes[1][3] = 15;
	this->m_cellSizes[1][4] = 10;
	this->m_cellSizes[1][5] = 5;
}

void Graph::mousePressEvent(QMouseEvent *me)
{
	const auto current = itemAt(me->pos());

    if (current == nullptr)
        return;

    auto selected = this->m_cellList->value(current);

    if (selected == nullptr)
        return;

    // Set and unset walls at the clicked vertex
    if (me->button() == Qt::LeftButton)
    {
        if (selected->IsWall())
        {
            selected->UnsetWall();
        }
        else
        {
            selected->SetWall();
        }
    }
}

void Graph::InitUI()
{
	auto GraphLayout = new QGridLayout();
    this->m_currentTab->setLayout(GraphLayout);

    // Graphics view for squares
    setGeometry(0, 0, this->m_sceneWidth, this->m_sceneHeight);
    this->m_scene = new QGraphicsScene();
    setScene(this->m_scene);
    GraphLayout->addWidget(this, 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignTop);

    // Spacers
	const auto spacer = new QSpacerItem(10, 0);
    GraphLayout->addItem(spacer, 0, 1);

    // Control Boxes
	const auto GraphGroupBox = new QGroupBox("Configuration", this);
    GraphLayout->addWidget(GraphGroupBox, 0, 2);
    AddItemsToGroupBox(GraphGroupBox);
}

void Graph::AddItemsToGroupBox(QGroupBox *groupBox)
{
    // Setting layouts
    auto controlLayout = new QFormLayout();
    groupBox->setLayout(controlLayout);

    const auto algoDescription = new QLabel("Algorithm");
    this->m_algorithmSelection = new QComboBox();
    this->m_algorithmSelection->addItem("Depth-First Search");
    this->m_algorithmSelection->addItem("Breadth-First Search");
    controlLayout->addRow(algoDescription, this->m_algorithmSelection);

    // Display the possible sizes of the Graph to user
    const auto GraphSizeDesc = new QLabel("Graph Size");
    this->m_sizeSelection = new QComboBox();
	for (auto i = 0; i < 6; i++)
	{
		this->m_sizeSelection->addItem(QString::number(this->m_cellSizes[0][i]));
	}
    controlLayout->addRow(GraphSizeDesc, this->m_sizeSelection);

    this->m_resetGraphButton = new QPushButton("Reset Graph");
    controlLayout->addRow(this->m_resetGraphButton);

    this->m_clearGraphButton = new QPushButton("Clear Graph");
    controlLayout->addRow(this->m_clearGraphButton);

	this->m_randomizeGraphButton = new QPushButton("Randomize Graph");
	controlLayout->addRow(this->m_randomizeGraphButton);

    this->m_startTravelButton = new QPushButton("Start Traveling");
    controlLayout->addRow(this->m_startTravelButton);

    this->m_stopTravelButton = new QPushButton("Stop Traveling");
    controlLayout->addRow(this->m_stopTravelButton);
	this->m_stopTravelButton->setVisible(false);

    // Connect UI objects to slots
	connect(this->m_sizeSelection, SIGNAL(activated(int)), this, SLOT(NewSize()));
    connect(this->m_startTravelButton, SIGNAL(clicked()), this, SLOT(StartTraveling()));
    connect(this->m_stopTravelButton, SIGNAL(clicked()), this, SLOT(StopTraveling()));
    connect(this->m_resetGraphButton, SIGNAL(clicked()), this, SLOT(Reset()));
    connect(this->m_clearGraphButton, SIGNAL(clicked()), this, SLOT(Clear()));
	connect(this->m_randomizeGraphButton, SIGNAL(clicked()), this, SLOT(Randomize()));
}

void Graph::AddItemsToScene() const
{
	const auto cols = this->m_sceneWidth / this->m_cellSize;
    const auto rows = this->m_sceneHeight / this->m_cellSize;
    auto i = 0;
    auto j = 0;
    auto idCount = 0;

    // Traverse rows/cols
    while (i <  rows)
    {
        while (j < cols)
        {
            // Get a new vertex
            auto vertex = new Vertex(idCount, j * this->m_cellSize, i * this->m_cellSize, this->m_cellSize);
            this->m_scene->addItem(vertex->GetShape());

            // Insert vertices into hash tables
            this->m_cellList->insert(vertex->GetShape(), vertex);
            this->m_idList->insert(idCount, vertex);

            // Set vertex descriptions for the smaller sizes
            if (this->m_cellSize > this->m_vertexDescThreshold)
                vertex->SetDescription();

            j++;
            idCount++;
        }
        j = 0;
        i++;
    }
    SetStartAndGoal();
}

void Graph::SetStartAndGoal() const
{
    this->m_idList->value(0)->SetStart(true);
    this->m_idList->value(this->m_idList->size() - 1)->SetGoal(true);
}

void Graph::SetDefaultSelections()
{
    // Set default Graph size
    this->m_sizeSelection->setCurrentIndex(1);
    this->m_cellSize = this->m_cellSizes[1][this->m_sizeSelection->currentIndex()];

    // Set default algorithm
    this->m_algorithmSelection->setCurrentIndex(1);
}

int Graph::TracePath(Vertex* lastVertex, QStack<int>* stack)
{
	auto count = 0;
	while (lastVertex != nullptr)
	{
		lastVertex->TracePath();
		stack->push(lastVertex->GetId());
		lastVertex = lastVertex->GetPrevious();
		count++;
	}
	return count;
}

void Graph::UpdateUiState()
{
	this->m_startTravelButton->setEnabled(!this->m_startTravelButton->isEnabled());
	this->m_resetGraphButton->setEnabled(!this->m_resetGraphButton->isEnabled());
	this->m_sizeSelection->setEnabled(!this->m_sizeSelection->isEnabled());
	this->m_algorithmSelection->setEnabled(!this->m_algorithmSelection->isEnabled());
	this->m_clearGraphButton->setEnabled(!this->m_clearGraphButton->isEnabled());
	this->m_randomizeGraphButton->setEnabled(!this->m_randomizeGraphButton->isEnabled());
}

void Graph::Render()
{
    AddItemsToScene();
}

void Graph::NewSize()
{
    if (this->m_cellSize == this->m_cellSizes[1][this->m_sizeSelection->currentIndex()])
        return;

    this->m_cellSize = this->m_cellSizes[1][this->m_sizeSelection->currentIndex()];
    this->m_scene->clear();

    for (auto &vertex : *this->m_cellList)
        delete vertex;

    this->m_cellList->clear();
    this->m_idList->clear();
    AddItemsToScene();
    this->m_startTravelButton->setEnabled(true);
}

void Graph::StartTraveling()
{
	UpdateUiState();
	this->m_startTravelButton->setVisible(false);
	this->m_stopTravelButton->setVisible(true);

	const auto cols = this->m_sceneWidth / this->m_cellSize;
	const auto rows = this->m_sceneHeight / this->m_cellSize;

	this->m_pathFinder->Setup(this->m_idList, rows, cols);

	if (this->m_algorithmSelection->currentText() == "Depth-First Search")
	{
		this->m_pathFinder->StartDepthFirstSearch();
	}
	else if (this->m_algorithmSelection->currentText() == "Breadth-First Search")
	{
		this->m_pathFinder->StartBreadthFirstSearch();
	}
}

void Graph::StopTraveling()
{
	m_stopTravelButton->setVisible(false);
	m_startTravelButton->setEnabled(true);
	m_pathFinder->TriggerInterrupt();
}

void Graph::Reset()
{
    for (auto& vertex : *this->m_cellList)
    {
	    if (!vertex->IsWall())
	    {
			vertex->UnsetWall();
			vertex->SetStart(false);
			vertex->SetVisited(false);
			vertex->SetGoal(false);

			if (this->m_cellSize > this->m_vertexDescThreshold)
				vertex->SetDescription();
	    }
    }

	// Reset start/goal
	SetStartAndGoal();

	this->m_startTravelButton->setEnabled(true);
}

void Graph::Clear()
{
    for (auto& vertex : *this->m_cellList)
    {
		vertex->UnsetWall();
		vertex->SetStart(false);
		vertex->SetVisited(false);
		vertex->SetGoal(false);

		if (this->m_cellSize > this->m_vertexDescThreshold)
			vertex->SetDescription();
    }

	// Reset start/goal
	SetStartAndGoal();

	this->m_startTravelButton->setEnabled(true);
}

void Graph::Randomize()
{
	Clear();
	for (auto vertex : *this->m_cellList)
	{
		if (!vertex->IsGoal() || !vertex->IsStart())
		{
			const auto heuristic = qrand() % 3; // Decides if it's a wall or not
			if (heuristic >= 2)			
				vertex->SetWall();			
		}
	}
}

void Graph::DisplayResults(Vertex* vertex)
{
	// Trace the path
	if (vertex != nullptr)
	{
		const auto path = new QStack<int>();
		const auto pathLength = TracePath(vertex, path);

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

	// Disable searching until Graph is reset
	UpdateUiState();
	this->m_startTravelButton->setEnabled(false);
	this->m_startTravelButton->setVisible(true);
	this->m_stopTravelButton->setVisible(false);
}
