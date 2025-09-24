add_library(DSU DSU.cpp DSU.h)

target_include_directories(DSU PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/Graph
)