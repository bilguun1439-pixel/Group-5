#ifndef COINPATH_MAZEERROR_H
#define COINPATH_MAZEERROR_H

#include <stdexcept>

// Thrown when a maze cannot be produced: a file that is missing or
// unreadable, a layout that breaks the format, or a maze that fails
// validation (for example an exit that cannot be reached). The message is
// written for the player, so the UI can show it as-is.
class MazeError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

#endif // COINPATH_MAZEERROR_H
