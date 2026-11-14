#ifndef COINPATH_POSITION_H
#define COINPATH_POSITION_H

// A cell coordinate in the maze grid, and the four directions the player can
// move in. Kept as a small value type so every class can share it freely.

struct Position {
    int row = 0;
    int col = 0;
};

inline bool operator==(Position a, Position b) {
    return a.row == b.row && a.col == b.col;
}

inline bool operator!=(Position a, Position b) {
    return !(a == b);
}

enum class Direction { Up, Down, Left, Right };

// The neighbouring cell one step from `from` in direction `direction`.
// No bounds checking: the Maze decides whether the result is a legal cell.
inline Position step(Position from, Direction direction) {
    switch (direction) {
    case Direction::Up:    return {from.row - 1, from.col};
    case Direction::Down:  return {from.row + 1, from.col};
    case Direction::Left:  return {from.row, from.col - 1};
    case Direction::Right: return {from.row, from.col + 1};
    }
    return from;
}

#endif // COINPATH_POSITION_H
