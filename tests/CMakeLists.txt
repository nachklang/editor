project(core_tests)

file(GLOB PROJECT_SOURCES "*.h" "*.cpp")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

find_package (Threads)
target_link_libraries(${PROJECT_NAME} editor_core gtest ${CMAKE_THREAD_LIBS_INIT})
