cmake_minimum_required (VERSION 3.6)
project(example)

add_subdirectory(/home/pi/sw/sleepy-discord /home/pi/sw/sleepy-discord/build)

add_executable(ParanoidBOT main.cpp Botclient.cpp MessageParser.cpp UserDatabase.cpp ResetTimer.cpp MessageHistory.cpp)

target_link_libraries(ParanoidBOT sleepy-discord)
