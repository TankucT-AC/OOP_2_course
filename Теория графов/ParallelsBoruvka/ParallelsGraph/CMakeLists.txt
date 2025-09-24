add_library(ParallelsGraph ParallelsGraph.h ParallelsGraph.cpp)

find_package(TBB REQUIRED)
target_link_libraries(ParallelsGraph PRIVATE TBB::tbb)

target_include_directories(ParallelsGraph PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/Graph
	${PROJECT_SOURCE_DIR}/DSU
)