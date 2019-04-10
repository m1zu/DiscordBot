cmake_minimum_required (VERSION 3.6)
project(example)

add_subdirectory(sleepy-discord)

add_executable(ParanoidBot main.cpp Botclient.cpp MessageParser.cpp Userdatabase.cpp ResetTimer.cpp MessageHistory.cpp)

target_link_libraries(ParanoidBot sleepy-discord)
