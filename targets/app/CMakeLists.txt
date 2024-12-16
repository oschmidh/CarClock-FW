cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES CXX)

target_include_directories(app PRIVATE include)

target_link_libraries(app
PUBLIC
)

target_sources(app
PRIVATE
    src/main.cpp
)
