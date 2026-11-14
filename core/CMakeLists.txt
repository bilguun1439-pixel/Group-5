# core/ - the game's domain logic as a plain C++ library.
# It must never depend on Qt or on ui/, so it builds (and its tests run)
# without the user interface.

add_library(coinpath_core STATIC
    Position.h
    GameTypes.h
    MazeError.h
    Wallet.h            Wallet.cpp
    Door.h              Door.cpp
    Graph.h             Graph.cpp
    Maze.h              Maze.cpp
    MazeBuilder.h       MazeBuilder.cpp
    MazeParser.h        MazeParser.cpp
    IMazeSource.h
    FileMazeSource.h    FileMazeSource.cpp
    IClock.h
    GameTimer.h         GameTimer.cpp
    Player.h            Player.cpp
    IGameObserver.h
    GameSession.h       GameSession.cpp
)

target_include_directories(coinpath_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(coinpath_core PUBLIC cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(coinpath_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
