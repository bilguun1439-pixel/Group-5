# tests/ - GoogleTest suite for core/. Links only coinpath_core, which shows
# that the domain builds and runs without the Qt interface.

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# Use the same C runtime as the rest of the project on Windows.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(coinpath_tests
    FakeClock.h
    WalletTest.cpp
    DoorTest.cpp
    GraphTest.cpp
    MazeBuilderTest.cpp
    MazeParserTest.cpp
    GameTimerTest.cpp
    GameSessionTest.cpp
    FileMazeSourceTest.cpp
)

target_link_libraries(coinpath_tests PRIVATE coinpath_core GTest::gtest_main GTest::gmock)
target_compile_definitions(coinpath_tests PRIVATE
    COINPATH_MAZE_DIR="${PROJECT_SOURCE_DIR}/assets/mazes")

include(GoogleTest)
gtest_discover_tests(coinpath_tests)
