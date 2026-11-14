#ifndef MAZE_H
#include <string>
#include <vector>
#include "Graph.h" // Include your Graph, Node, and Edge classes

enum class Cell { Empty, Wall }; // Example cell type definition

class Maze {
private:
    std::string name;
    int rows;
    int cols;
    int startRow;
    int startCol;
    std::vector<std::vector<Cell>> grid;
    Graph graph; // The graph representation of the maze

    // Helper to convert 2D coordinates to a 1D node number
    int getCellIndex(int r, int c) const {
        return r * cols + c;
    }

public:
    Maze(const std::string& name, int rows, int cols, int startRow, int startCol);

    const std::string& getName() const;
    int getRows() const;
    int getCols() const;
    int getStartRow() const;
    int getStartCol() const;
    Graph& getGraph(); /
};

#endif