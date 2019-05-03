#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	this->m_windowWidth = 1100;
	this->m_windowHeight = 700;

	this->setFixedSize(m_windowWidth, m_windowHeight);

	this->InitUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* ke)
{

	// Close window on ESC
	if (ke->key() == Qt::Key_Escape)
	{
		this->window()->close();
	}
}

void MainWindow::InitUI()
{
    // Load layout from form
    ui->setupUi(this);

    // Set the size of the window
    this->setMinimumHeight(m_windowHeight);
    this->setMinimumWidth(m_windowWidth);

    // Set the size of the tab widget within the window
    ui->tabWidget->setMinimumHeight(m_windowHeight - 100);
    ui->tabWidget->setMinimumWidth(m_windowWidth - 100);

    // Set Mouse tracking
    this->setMouseTracking(true);
    ui->tabWidget->setMouseTracking(true);

    // Initialize the grid
    m_grid = new Graph(ui->tabWidget->widget(0));
}
