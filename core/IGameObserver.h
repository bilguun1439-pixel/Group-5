#ifndef COINPATH_IGAMEOBSERVER_H
#define COINPATH_IGAMEOBSERVER_H

#include "GameTypes.h"
#include "Position.h"

// IGameObserver - the Observer in the Observer pattern (behavioural).
//
// GameSession (the subject) announces every change through this interface
// and knows nothing else about who is listening. The Qt view implements it
// to redraw; tests can implement it to record events. Every method has an
// empty default, so a listener overrides only the events it cares about.
class IGameObserver {
public:
    virtual ~IGameObserver() = default;

    virtual void onPlayerMoved(Position /*position*/) {}
    virtual void onCoinsChanged(int /*coins*/) {}
    virtual void onTimeChanged(int /*remainingSeconds*/) {}
    virtual void onStatusChanged(GameStatus /*status*/) {}
    virtual void onMoveRejected(MoveResult /*reason*/) {}
};

#endif // COINPATH_IGAMEOBSERVER_H
