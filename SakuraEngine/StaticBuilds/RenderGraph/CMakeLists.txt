cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(RenderGraph 
VERSION 1.00
LANGUAGES CXX
)

if(${SAKURA_UNITY_BUILD})
add_library(RenderGraph
STATIC
RenderGraph_All.cpp)
else()
add_library(RenderGraph
STATIC
RenderGraph.cpp
GraphPassNode.cpp
GraphResourceNode.cpp)
endif()