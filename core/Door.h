#ifndef COINPATH_DOOR_H
#define COINPATH_DOOR_H

#include "Position.h"

class Wallet;

// Door - a cell the player can only pass once the door is open (FR2).
//
// Door kinds differ only in what opening them does to the wallet, so that
// step is the single virtual hook (Template Method): tryPass() holds the
// shared rule "an open door is free to walk through; a closed one must be
// opened first", and each subclass supplies applyOpening(). Adding a new door
// kind means adding a subclass, with no change to Door or GameSession.
class Door {
public:
    virtual ~Door() = default;

    Door(const Door&) = delete;
    Door& operator=(const Door&) = delete;

    Position getPosition() const;
    bool isOpen() const;

    // Lets the player through, opening the door first if it is closed.
    // Returns false, leaving the door closed and the wallet untouched, when
    // the door cannot be opened.
    bool tryPass(Wallet& wallet);

    // Coins gained (positive) or spent (negative) by opening this door. Used
    // for display and as the edge weight in the maze graph.
    virtual int getCoinChange() const = 0;

protected:
    explicit Door(Position position);

private:
    // Applies this door's coin effect. Returns false if it cannot be applied.
    virtual bool applyOpening(Wallet& wallet) = 0;

    Position position;
    bool open = false;
};

// A door that costs coins to open (FR3).
class PaidDoor final : public Door {
public:
    // Throws std::invalid_argument unless cost is positive.
    PaidDoor(Position position, int cost);

    int getCost() const;
    int getCoinChange() const override;

private:
    bool applyOpening(Wallet& wallet) override;

    int cost;
};

// A door that gives coins when opened (FR7).
class RewardDoor final : public Door {
public:
    // Throws std::invalid_argument unless reward is positive.
    RewardDoor(Position position, int reward);

    int getReward() const;
    int getCoinChange() const override;

private:
    bool applyOpening(Wallet& wallet) override;

    int reward;
};

#endif // COINPATH_DOOR_H
