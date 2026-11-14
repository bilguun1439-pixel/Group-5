#ifndef COINPATH_PLAYER_H
#define COINPATH_PLAYER_H

#include "Position.h"
#include "Wallet.h"

// Player - the character the user controls: where it is, and its coins.
// Player does not decide whether a move is legal; GameSession does that by
// asking the Maze, then calls moveTo().
class Player {
public:
    Player(Position start, int startingCoins);

    Position getPosition() const;
    void moveTo(Position destination);

    Wallet& getWallet();
    const Wallet& getWallet() const;

private:
    Position position;
    Wallet wallet;
};

#endif // COINPATH_PLAYER_H
