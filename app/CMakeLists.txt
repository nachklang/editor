project(editor)

file(GLOB_RECURSE PROJECT_SOURCES "*.h" "*.cpp" "*.qrc")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} resources.qrc)

set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets)
find_package(Qt5Svg)
target_link_libraries(${PROJECT_NAME} editor_core Qt5::Widgets)

#add_custom_target(copy-files ALL
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_BINARY_DIR}/app/images/
#    ${CMAKE_BINARY_DIR}/app/images/
#    )
