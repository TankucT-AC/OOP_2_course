add_executable(src main_boruvka.cpp)

target_link_libraries(src PUBLIC Graph DSU ParallelsGraph)
target_include_directories(src PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Graph
    ${PROJECT_SOURCE_DIR}/DSU
    ${PROJECT_SOURCE_DIR}/ParallelsGraph    
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET src PROPERTY CXX_STANDARD 20)
endif()
