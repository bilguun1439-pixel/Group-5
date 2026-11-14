#include "Door.h"

//Constructor for the Door class
Door::Door(int row, int col, int coinValue, bool rewardDoor)
    : row(row), col(col), coinValue(coinValue), rewardDoor(rewardDoor), open(false) {}

//get the row of the door
int Door::getRow() const {
    return row;
}
// the getter method for the column of the door
int Door::getCol() const {
    return col;
}
// the getter method for the number of coins needed to open the door
int Door::getCoinValue() const {
    return coinValue;
}
// getter method that returns whether the door rewards coins
bool Door::isRewardDoor() const {
    return rewardDoor;
}
// method that returns whether a door is open
bool Door::isOpen() const {
    return open;
}
