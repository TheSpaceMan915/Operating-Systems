cmake_minimum_required(VERSION 3.23)
project(Operating_Systems)

set(CMAKE_CXX_STANDARD 23)

add_executable(Operating_Systems main.cpp)
add_executable(runTask4Client Lab4/cpp/task44_client.cpp)
add_executable(runTask4Server Lab4/cpp/task44_server.cpp)

add_executable(runTask5Client Lab4/cpp/task45_client.cpp)
add_executable(runTask5Server Lab4/cpp/task45_server.cpp)

add_executable(runTask6 Lab4/cpp/task46.cpp)

add_executable(runTask7Server Lab4/cpp/task47_server.cpp)
add_executable(runTask7Client Lab4/cpp/task47_client.cpp)

add_executable(runTask8 Lab4/cpp/task48.cpp)

add_executable(runLab5 Lab5/drive_block_space.cpp)