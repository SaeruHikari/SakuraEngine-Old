cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

find_package(pegtl CONFIG REQUIRED)

project(SakuraParser 
  VERSION 1.00
  LANGUAGES CXX
)

add_library(SakuraParser 
STATIC
SakuraParser.cpp)

add_executable(ParserTest Test.cpp)

target_link_libraries(ParserTest taocpp::pegtl)