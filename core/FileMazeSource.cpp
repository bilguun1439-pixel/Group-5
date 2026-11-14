#include "FileMazeSource.h"

#include <algorithm>
#include <fstream>
#include <system_error>
#include <utility>

#include "MazeError.h"
#include "MazeParser.h"

namespace {
const char* const kExtension = ".maze";
const char* const kNameKey = "name:";

// Reads just the "name:" line, so listing mazes stays cheap and a broken
// layout only fails when that maze is actually chosen.
std::string readDisplayName(const std::filesystem::path& file, const std::string& fallback) {
    std::ifstream input(file);
    std::string line;
    while (std::getline(input, line)) {
        if (line.rfind(kNameKey, 0) == 0) {
            const auto start = line.find_first_not_of(" \t", std::string(kNameKey).size());
            const auto end = line.find_last_not_of(" \t\r");
            if (start != std::string::npos && end != std::string::npos && end >= start) {
                return line.substr(start, end - start + 1);
            }
        }
    }
    return fallback;
}
} // namespace

FileMazeSource::FileMazeSource(std::filesystem::path directory)
    : directory(std::move(directory)) {}

std::vector<MazeInfo> FileMazeSource::listMazes() const {
    std::vector<MazeInfo> mazes;
    std::error_code error;
    for (const auto& entry : std::filesystem::directory_iterator(directory, error)) {
        if (entry.is_regular_file() && entry.path().extension() == kExtension) {
            const std::string id = entry.path().stem().string();
            mazes.push_back({id, readDisplayName(entry.path(), id)});
        }
    }
    std::sort(mazes.begin(), mazes.end(),
              [](const MazeInfo& a, const MazeInfo& b) { return a.id < b.id; });
    return mazes;
}

Maze FileMazeSource::loadMaze(const std::string& id) const {
    const std::filesystem::path file = pathFor(id);
    std::ifstream input(file);
    if (!input) {
        throw MazeError("Could not open maze file '" + file.filename().string() + "'");
    }
    try {
        return MazeParser().parse(input);
    } catch (const MazeError& error) {
        throw MazeError(file.filename().string() + ": " + error.what());
    }
}

std::filesystem::path FileMazeSource::pathFor(const std::string& id) const {
    return directory / (id + kExtension);
}
