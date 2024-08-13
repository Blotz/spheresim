cmake_minimum_required(VERSION 3.10)
project(spheresim)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(include)

# Find dependencies
find_package(Catch2 REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# Add the executable
add_executable(spheresim 
    src/main.cpp
    src/sphere_simulation.cpp 
    src/sphere.cpp
)

# Link the libraries to the executable
target_link_libraries(spheresim Eigen3::Eigen sfml-graphics sfml-window sfml-system)

# Add tests
add_executable(tests tests/test_main.cpp)
target_link_libraries(tests Catch2::Catch2WithMain Eigen3::Eigen sfml-graphics sfml-window sfml-system)

# Enable testing
enable_testing()
add_test(NAME sphere_tests COMMAND tests)