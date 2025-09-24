# CMakeList.txt: проект CMake для Лабораторная работа №1; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Point "main.cpp" "Point.h" "Point.cpp" "Camera.h" "Camera.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Point PROPERTY CXX_STANDARD 20)
endif()

