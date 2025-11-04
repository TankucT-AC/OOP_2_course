add_executable(src main.cpp Point.cpp Point.h Camera.h Camera.cpp "CollisionWrapper.h" "CollisionWrapper.cpp")

target_include_directories(src PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET src PROPERTY CXX_STANDARD 20)
endif()

