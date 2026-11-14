#include "Player.h"

Player::Player(Position start, int startingCoins) : position(start), wallet(startingCoins) {}

Position Player::getPosition() const {
    return position;
}

void Player::moveTo(Position destination) {
    position = destination;
}

Wallet& Player::getWallet() {
    return wallet;
}

const Wallet& Player::getWallet() const {
    return wallet;
}
