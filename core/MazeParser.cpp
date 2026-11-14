#include "MazeParser.h"

#include <cctype>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "MazeBuilder.h"
#include "MazeError.h"

namespace {

constexpr char kCommentMarker = ';';

struct DoorSpec {
    bool reward = false;
    int amount = 0;
};

struct Header {
    std::string name;
    std::optional<int> coins;
    std::optional<int> timeSeconds;
    std::map<char, DoorSpec> doors;
};

std::string trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r");
    if (first == std::string::npos) {
        return "";
    }
    const auto last = text.find_last_not_of(" \t\r");
    return text.substr(first, last - first + 1);
}

[[noreturn]] void fail(int lineNumber, const std::string& message) {
    throw MazeError("Line " + std::to_string(lineNumber) + ": " + message);
}

int parseInt(const std::string& text, int lineNumber) {
    try {
        std::size_t used = 0;
        const int value = std::stoi(text, &used);
        if (used != text.size()) {
            fail(lineNumber, "'" + text + "' is not a whole number");
        }
        return value;
    } catch (const std::logic_error&) {
        fail(lineNumber, "'" + text + "' is not a whole number");
    }
}

void parseDoorLine(const std::string& rest, int lineNumber, Header& header) {
    std::istringstream words(rest);
    std::string letter;
    std::string kind;
    std::string amount;
    std::string extra;
    if (!(words >> letter >> kind >> amount) || (words >> extra)) {
        fail(lineNumber, "expected 'door <letter> paid|reward <amount>'");
    }
    if (letter.size() != 1 || !std::islower(static_cast<unsigned char>(letter[0]))) {
        fail(lineNumber, "a door is named by one lowercase letter");
    }
    if (kind != "paid" && kind != "reward") {
        fail(lineNumber, "door kind must be 'paid' or 'reward', not '" + kind + "'");
    }
    if (header.doors.count(letter[0]) != 0) {
        fail(lineNumber, "door '" + letter + "' is declared twice");
    }
    header.doors[letter[0]] = DoorSpec{kind == "reward", parseInt(amount, lineNumber)};
}

void parseHeaderLine(const std::string& line, int lineNumber, Header& header) {
    if (line.rfind("door ", 0) == 0) {
        parseDoorLine(line.substr(5), lineNumber, header);
        return;
    }
    const auto colon = line.find(':');
    if (colon == std::string::npos) {
        fail(lineNumber, "expected 'key: value', a door line, or 'grid:'");
    }
    const std::string key = trim(line.substr(0, colon));
    const std::string value = trim(line.substr(colon + 1));
    if (key == "name") {
        header.name = value;
    } else if (key == "coins") {
        header.coins = parseInt(value, lineNumber);
    } else if (key == "time") {
        header.timeSeconds = parseInt(value, lineNumber);
    } else {
        fail(lineNumber, "unknown setting '" + key + "'");
    }
}

} // namespace

Maze MazeParser::parse(std::istream& input) const {
    Header header;
    std::vector<std::string> rows;
    int gridStartLine = 0;
    bool inGrid = false;

    std::string rawLine;
    int lineNumber = 0;
    while (std::getline(input, rawLine)) {
        ++lineNumber;
        const std::string line = trim(rawLine);
        if (inGrid) {
            if (!line.empty()) {
                rows.push_back(line);
            }
        } else if (line == "grid:") {
            inGrid = true;
            gridStartLine = lineNumber + 1;
        } else if (!line.empty() && line.front() != kCommentMarker) {
            parseHeaderLine(line, lineNumber, header);
        }
    }

    if (header.name.empty()) {
        throw MazeError("The maze has no 'name:' line");
    }
    if (!header.coins) {
        throw MazeError("The maze has no 'coins:' line");
    }
    if (!header.timeSeconds) {
        throw MazeError("The maze has no 'time:' line");
    }
    if (!inGrid || rows.empty()) {
        throw MazeError("The maze has no grid");
    }

    const int rowCount = static_cast<int>(rows.size());
    const int colCount = static_cast<int>(rows.front().size());
    MazeBuilder builder(header.name, rowCount, colCount);
    builder.setStartingCoins(*header.coins).setTimeLimitSeconds(*header.timeSeconds);

    for (int row = 0; row < rowCount; ++row) {
        const int fileLine = gridStartLine + row;
        if (static_cast<int>(rows[row].size()) != colCount) {
            fail(fileLine, "every grid row must be " + std::to_string(colCount) + " cells wide");
        }
        for (int col = 0; col < colCount; ++col) {
            const char symbol = rows[row][col];
            const Position cell{row, col};
            if (symbol == '.') {
                continue;
            } else if (symbol == '#') {
                builder.addWall(cell);
            } else if (symbol == 'S') {
                builder.setStart(cell);
            } else if (symbol == 'E') {
                builder.setExit(cell);
            } else if (header.doors.count(symbol) != 0) {
                const DoorSpec& door = header.doors.at(symbol);
                if (door.reward) {
                    builder.addRewardDoor(cell, door.amount);
                } else {
                    builder.addPaidDoor(cell, door.amount);
                }
            } else {
                fail(fileLine, std::string("unknown grid symbol '") + symbol + "'");
            }
        }
    }
    return builder.build();
}
