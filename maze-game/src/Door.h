#ifndef DOOR_H
#define DOOR_H

// Door - a door located in a maze cell.
//
// Responsibility: store a door's position, its coin value, whether it is
// open, and whether it is a "reward" door. A normal door costs coins to open;
// a reward door gives coins instead (FR7). A single class with a flag covers
// both kinds, so no subclasses are needed.
//
// Future work (not implemented yet):
//  - opening a door and charging/awarding coins (FR3, FR7)
//  - closing a door and reversing the coin change on backtrack (FR4, FR7)
class Door {
public:
    Door(int row, int col, int coinValue, bool rewardDoor);

    int getRow() const;
    int getCol() const;
    int getCoinValue() const;
    bool isRewardDoor() const;
    bool isOpen() const;

private:
    int row;
    int col;
    int coinValue;
    bool rewardDoor;
    bool open;
};

#endif // DOOR_H
