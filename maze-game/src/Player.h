#ifndef PLAYER_H
#define PLAYER_H

// Player - the character controlled by the user.
//
// Responsibility: store the player's position in the maze grid and their
// coin inventory. Player only holds state; rules about where the player may
// move (walls, doors) are checked by Game/Maze.
//
// Future work (not implemented yet):
//  - movement in four directions (FR1)
//  - spending and recovering coins at doors (FR3, FR4, FR7)
class Player {
public:
    Player(int startRow, int startCol, int startingCoins);

    int getRow() const;
    int getCol() const;
    int getCoins() const;

private:
    int row;
    int col;
    int coins;
};

#endif // PLAYER_H
