project(editor)

file(GLOB_RECURSE PROJECT_SOURCES "*.h" "*.cpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

find_package(Qt5Widgets)

target_link_libraries(${PROJECT_NAME} editor_core Qt5::Widgets)

file(COPY objects DESTINATION ${CMAKE_BINARY_DIR}/app/)
file(COPY levels DESTINATION ${CMAKE_BINARY_DIR}/app/)
