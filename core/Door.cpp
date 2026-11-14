#include "Door.h"

#include <stdexcept>

#include "Wallet.h"

Door::Door(Position position) : position(position) {}

Position Door::getPosition() const {
    return position;
}

bool Door::isOpen() const {
    return open;
}

bool Door::tryPass(Wallet& wallet) {
    if (open) {
        return true;
    }
    if (!applyOpening(wallet)) {
        return false;
    }
    open = true;
    return true;
}

PaidDoor::PaidDoor(Position position, int cost) : Door(position), cost(cost) {
    if (cost <= 0) {
        throw std::invalid_argument("A paid door must cost at least one coin");
    }
}

int PaidDoor::getCost() const {
    return cost;
}

int PaidDoor::getCoinChange() const {
    return -cost;
}

bool PaidDoor::applyOpening(Wallet& wallet) {
    return wallet.debit(cost);
}

RewardDoor::RewardDoor(Position position, int reward) : Door(position), reward(reward) {
    if (reward <= 0) {
        throw std::invalid_argument("A reward door must give at least one coin");
    }
}

int RewardDoor::getReward() const {
    return reward;
}

int RewardDoor::getCoinChange() const {
    return reward;
}

bool RewardDoor::applyOpening(Wallet& wallet) {
    wallet.credit(reward);
    return true;
}
