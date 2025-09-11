# CMakeList.txt: проект CMake для Лабораторная работа №1; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (CMakeTarget "main_graph.cpp" "Graph.h" "Graph.cpp"  )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

add_custom_command(TARGET CMakeTarget POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Тесты
    $<TARGET_FILE_DIR:CMakeTarget>/Тесты
    COMMENT "Copying Тесты files to output directory"
)