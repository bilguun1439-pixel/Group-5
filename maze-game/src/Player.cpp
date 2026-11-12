#include "Player.h"

Player::Player(int startRow, int startCol, int startingCoins)
    : row(startRow), col(startCol), coins(startingCoins) {}

int Player::getRow() const {
    return row;
}

int Player::getCol() const {
    return col;
}

int Player::getCoins() const {
    return coins;
}
