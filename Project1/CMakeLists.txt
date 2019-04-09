cmake_minimum_required (VERSION 3.6)
project(example)

add_subdirectory(sleepy-discord)

add_executable(ParanoidBOT main.cpp Botclient.cpp MessageParser.cpp Userdatabase.cpp)

target_link_libraries(ParanoidBOT sleepy-discord)
