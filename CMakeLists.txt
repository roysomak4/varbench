cmake_minimum_required(VERSION 3.16)

project(varbench)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -static")

include_directories(include)
link_directories(lib)

# # gzstream library needs to be installed on the system
# # from https://github.com/roysomak4/gzstream
# find_library(gzlib gzstream)
# # zlib has to be compiled statically and installed in system path
# find_library(zlib z)

file(GLOB SOURCES "src/*.c*")

add_executable(varbench ${SOURCES})

# target_link_libraries(varbench ${gzlib} ${zlib})
target_link_libraries(varbench gzstream z)
