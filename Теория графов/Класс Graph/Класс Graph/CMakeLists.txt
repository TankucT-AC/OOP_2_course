add_executable (CMakeTarget "main_graph.cpp" "Graph.h" "Graph.cpp"  )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()