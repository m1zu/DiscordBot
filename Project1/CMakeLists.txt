cmake_minimum_required (VERSION 3.6)
project(example)

add_subdirectory(sleepy-discord)

add_executable(hello main.cpp Botclient.cpp)

target_link_libraries(hello sleepy-discord)
