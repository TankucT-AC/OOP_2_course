add_executable (src "src.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET src PROPERTY CXX_STANDARD 20)
endif()
