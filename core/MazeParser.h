#ifndef COINPATH_MAZEPARSER_H
#define COINPATH_MAZEPARSER_H

#include <istream>

#include "Maze.h"

// MazeParser - the Director in the Builder pattern.
//
// Reads the text format of a .maze file and drives a MazeBuilder with it.
// It knows the file format; MazeBuilder knows what makes a maze valid.
// Throws MazeError, naming the line, for anything it cannot understand.
//
// Format (see assets/mazes/ for examples):
//   name: Warm-up            header lines, in any order
//   coins: 3
//   time: 60
//   door a paid 2            a lowercase letter, "paid" or "reward", an amount
//   ; a comment              header lines starting with ';' are ignored
//   grid:                    everything after this line is the layout
//   #######                  # wall   . floor   S start   E exit
//   #S.a.E#                  a-z  a door declared above
//   #######
class MazeParser {
public:
    Maze parse(std::istream& input) const;
};

#endif // COINPATH_MAZEPARSER_H
