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
    void InitUI();
private:
    Ui::MainWindow *ui;
    Grid *m_grid;
    int m_windowHeight;
    int m_windowWidth;
};

