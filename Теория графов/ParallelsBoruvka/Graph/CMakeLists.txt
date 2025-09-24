add_library(Graph Graph.cpp Graph.h)

target_include_directories(Graph PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/DSU
)