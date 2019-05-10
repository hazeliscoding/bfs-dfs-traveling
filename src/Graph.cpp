#include "Graph.h"
#include <random>
#include <array>

Graph::Graph(QWidget *parent)
	: QGraphicsView(parent), m_currentlyTraveling(false)
{
	this->m_currentTab = parent;

	// Default size of the Graph canvas
	this->m_sceneHeight = 600;
	this->m_sceneWidth = 750;

	// Load selectable Graph sizes
	// Row 1: Visible number of squares
	// Row 2: Size of squares
	this->m_sizeList = SizeList{
		{20, 150},
		{80, 75},
		{500, 30},
		{2000, 15},
		{4500, 10},
		{18000, 5},
	};

	// Default values
	this->m_cellSize = this->m_sizeList[0].second;
	this->m_vertexDescThreshold = this->m_sizeList[2].second;

	// Data structures which hold pointers to all the vertices
	this->m_vertices = new QHash<QGraphicsItem*, Vertex*>();
	this->m_vertexIdList = new QHash<int, Vertex*>();

	InitUI();
	SetDefaultSelections();
	Render();

	// Initialize pathfinder
	const auto cols = this->m_sceneWidth / this->m_cellSize;
	const auto rows = this->m_sceneHeight / this->m_cellSize;
	this->m_pathFinder = new PathFinder(this->m_vertexIdList, rows, cols);
	connect(this->m_pathFinder, SIGNAL(DisplayGoal(Vertex*)), this, SLOT(DisplayResults(Vertex*)));
}

void Graph::mousePressEvent(QMouseEvent *me)
{
	// Do not process event during traversal
	if (!m_currentlyTraveling)
		return;

	const auto current = itemAt(me->pos());

    if (current == nullptr)
        return;
	
    auto selected = this->m_vertices->value(current);

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
	for (auto& i : this->m_sizeList)
	{
		this->m_sizeSelection->addItem(QString::number(i.first));
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

void Graph::SetStartAndGoal() const
{
    this->m_vertexIdList->value(0)->SetStart(true);
    this->m_vertexIdList->value(this->m_vertexIdList->size() - 1)->SetGoal(true);
}

void Graph::SetDefaultSelections()
{
    // Set default Graph size
    this->m_sizeSelection->setCurrentIndex(1);
    this->m_cellSize = this->m_sizeList[this->m_sizeSelection->currentIndex()].second;

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
	this->m_currentlyTraveling = !this->m_currentlyTraveling;
}

void Graph::Render() const
{
	const auto cols = this->m_sceneWidth / this->m_cellSize;
	const auto rows = this->m_sceneHeight / this->m_cellSize;
	auto i = 0;
	auto j = 0;
	auto idCount = 0;

	// Traverse rows/cols
	while (i < rows)
	{
		while (j < cols)
		{
			// Get a new vertex
			auto vertex = new Vertex(idCount, j * this->m_cellSize, i * this->m_cellSize, this->m_cellSize);
			this->m_scene->addItem(vertex->GetShape());

			// Insert vertices into hash tables
			this->m_vertices->insert(vertex->GetShape(), vertex);
			this->m_vertexIdList->insert(idCount, vertex);

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

void Graph::NewSize()
{
    if (this->m_cellSize == this->m_sizeList[this->m_sizeSelection->currentIndex()].second)
        return;

    this->m_cellSize = this->m_sizeList[this->m_sizeSelection->currentIndex()].second;
    this->m_scene->clear();

    for (auto &vertex : *this->m_vertices)
        delete vertex;

    this->m_vertices->clear();
    this->m_vertexIdList->clear();
    Render();
    this->m_startTravelButton->setEnabled(true);
}

void Graph::StartTraveling()
{
	this->m_currentlyTraveling = true;

	UpdateUiState();
	this->m_startTravelButton->setVisible(false);
	this->m_stopTravelButton->setVisible(true);

	const auto cols = this->m_sceneWidth / this->m_cellSize;
	const auto rows = this->m_sceneHeight / this->m_cellSize;

	this->m_pathFinder->Setup(this->m_vertexIdList, rows, cols);

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
	this->m_currentlyTraveling = false;

	m_stopTravelButton->setVisible(false);
	m_startTravelButton->setEnabled(true);
	m_pathFinder->TriggerInterrupt();
}

void Graph::Reset() const
{
    for (auto& vertex : *this->m_vertices)
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

void Graph::Clear() const
{
    for (auto& vertex : *this->m_vertices)
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

void Graph::Randomize() const
{
	Clear();
	for (auto vertex : *this->m_vertices)
	{
		if (!vertex->IsGoal() || !vertex->IsStart())
		{
			const auto heuristic = rand() % 3; // Decides if it's a wall or not
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
