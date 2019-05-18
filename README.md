# Traveling in BFS/DFS
A fun and practical way to demonstrate Breadth-First Search and Depth-First Search with pathfinding on a grid for a second semester Data Structures and Algorithms univeristy course as a final project.  

## Project Objective

To implement a graph data structure which can be edited, and traversed through with Breadth-First Search and Depth-First Search trversals. In this implementation, I have implemented a graph as a typical 2D grid which consists of cells, which are the vertices of this graph. Movement between cells can be represented as edges. Impassable cells (walls) are not part of the graph. This graph is also unweighted and undirected. The implementation of the search algorithms do not factor in any distances or heuristics.

## Prerequisites
* [CMake](https://cmake.org/)
* [Qt5](https://www.qt.io/)

Also, make sure these packages are installed.
```shell
sudo apt-get install qtbase5-dev
sudo apt-get install qtdeclarative5-dev
```

## Building

``` shell
cd mkdir build && cd build
cmake ..
make
```
