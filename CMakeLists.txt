cmake_minimum_required(VERSION 3.16)

project(CoinPathDash VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# GoogleTest is downloaded at configure time, so the tests are opt-in:
#   cmake -S . -B build -DCOINPATH_BUILD_TESTS=ON
option(COINPATH_BUILD_TESTS "Build the GoogleTest suite in tests/" OFF)

add_subdirectory(core) # domain logic, no Qt
add_subdirectory(ui)   # Qt front end

if(COINPATH_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
