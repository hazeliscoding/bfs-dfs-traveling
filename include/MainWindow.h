#pragma once

#include <QMainWindow>

#include "Grid.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
	// Initializes ui for window
    void InitUI();
private:
	// UI entity
    Ui::MainWindow *ui;

	// Pointer to the grid GUI item
    Grid *m_grid;

	// Window height, width
    int m_windowHeight;
    int m_windowWidth;
};

