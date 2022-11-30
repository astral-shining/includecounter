cmake_minimum_required(VERSION 3.14)

project("includecounter" VERSION 0.1)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
message(${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_FLAGS "-Wall -pedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-s -flto -fuse-linker-plugin -O3 -DNDEBUG")

add_subdirectory(lib/fmt)
add_subdirectory(src)

add_executable(includecounter ${INCLUDECOUNTER_SOURCES})
target_link_libraries(includecounter PRIVATE fmt::fmt)