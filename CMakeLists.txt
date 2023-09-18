cmake_minimum_required(VERSION 3.27.3)
project(tic-tac-toe-cli)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)
link_libraries(OpenSSL::SSL OpenSSL::Crypto)

file(GLOB APP_SOURCES src/*.cpp)
file(GLOB HEADER_FILES headers/*.h)

add_executable(tic-tac-toe-cli ${APP_SOURCES} ${HEADER_FILES})

target_include_directories(tic-tac-toe-cli PRIVATE headers)
