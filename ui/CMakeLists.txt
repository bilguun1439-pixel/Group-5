# ui/ - the Qt front end. Depends on core/; core/ never depends on this.

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)

add_executable(CoinPathDash
    main.cpp
    MainWindow.h        MainWindow.cpp
    MazeSelectView.h    MazeSelectView.cpp
    GameView.h          GameView.cpp
    MazeBoard.h         MazeBoard.cpp
)

target_link_libraries(CoinPathDash PRIVATE coinpath_core Qt6::Widgets)

set_target_properties(CoinPathDash PROPERTIES WIN32_EXECUTABLE ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(CoinPathDash PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Put the maze files next to the executable, where main() looks for them.
add_custom_command(TARGET CoinPathDash POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/assets/mazes
            $<TARGET_FILE_DIR:CoinPathDash>/mazes
    COMMENT "Copying maze files next to the executable"
)
