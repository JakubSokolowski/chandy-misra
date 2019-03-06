cmake_minimum_required(VERSION 3.8)
project(chandy_misra)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -no-pie -pedantic -g")
find_package(Threads REQUIRED)
set(ADDITIONAL_LIBRARIES menu)
find_package(Curses REQUIRED)


set(SOURCE_FILES
        main.cpp
        Coordinator.h Coordinator.cpp
        Fork.h Fork.cpp
        Philosopher.h Philosopher.cpp
        Table.h Table.cpp
        ThreadSafePrinter.h ThreadSafePrinter.cpp
        Visualisation.h Visualisation.cpp)
add_executable(chandy_misra ${SOURCE_FILES})
target_link_libraries(chandy_misra Threads::Threads curses menu)