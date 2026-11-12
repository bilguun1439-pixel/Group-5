#include "Door.h"

Door::Door(int row, int col, int coinValue, bool rewardDoor)
    : row(row), col(col), coinValue(coinValue), rewardDoor(rewardDoor), open(false) {}

int Door::getRow() const {
    return row;
}

int Door::getCol() const {
    return col;
}

int Door::getCoinValue() const {
    return coinValue;
}

bool Door::isRewardDoor() const {
    return rewardDoor;
}

bool Door::isOpen() const {
    return open;
}
