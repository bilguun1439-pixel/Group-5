#ifndef COINPATH_GAMETYPES_H
#define COINPATH_GAMETYPES_H

// The life cycle of a game (DP3). Transitions, all made by GameSession:
//   NotStarted -> Running     start()
//   Running    -> Won         the player steps onto the exit
//   Running    -> TimedOut    the timer runs out
// Won and TimedOut are final. Any other transition is refused.
enum class GameStatus { NotStarted, Running, Won, TimedOut };

// What happened to one move request.
enum class MoveResult {
    Moved,           // the player moved (and may have opened a door or won)
    Blocked,         // a wall or the edge of the maze is in the way
    NotEnoughCoins,  // a closed paid door costs more than the player has
    GameNotRunning   // the game has not started or is already over
};

#endif // COINPATH_GAMETYPES_H
