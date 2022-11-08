cmake_minimum_required(VERSION 3.23)
project(Operating_Systems)

set(CMAKE_CXX_STANDARD 23)

add_executable(Operating_Systems main.cpp)
add_executable(runLab4Client Lab4/cpp/lab4_task5_client.cpp)
add_executable(runLab4Server Lab4/cpp/lab4_task5_server.cpp)