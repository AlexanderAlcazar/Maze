cmake_minimum_required(VERSION 3.30)
project(Maze)

set(CMAKE_CXX_STANDARD 11)

add_executable(Maze main.cpp maze.cpp room.cpp player.cpp entity.cpp game.cpp shark.cpp)

add_custom_command(
        TARGET Maze         # This command applies to the 'Maze' executable target
        POST_BUILD          # Execute this command after 'Maze' has been successfully built
        COMMAND ${CMAKE_COMMAND} -E copy # Use CMake's built-in, cross-platform 'copy' utility
        "${CMAKE_SOURCE_DIR}/settings.ini" # Source file: points to settings.ini in your project root
        "$<TARGET_FILE_DIR:Maze>/settings.ini" # Destination: The directory where 'Maze' executable is placed
        COMMENT "Copying settings.ini to build output directory..." # Message shown during build
)

add_custom_command(
        TARGET Maze         # This command applies to the 'Maze' executable target
        POST_BUILD          # Execute this command after 'Maze' has been successfully built
        COMMAND ${CMAKE_COMMAND} -E copy # Use CMake's built-in, cross-platform 'copy' utility
        "${CMAKE_SOURCE_DIR}/maze_lecture.txt" # Source file: points to settings.ini in your project root
        "$<TARGET_FILE_DIR:Maze>/maze_lecture.txt" # Destination: The directory where 'Maze' executable is placed
        COMMENT "Copying settings.ini to build output directory..." # Message shown during build
)