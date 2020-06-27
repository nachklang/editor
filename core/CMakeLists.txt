project(editor_core)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

find_package(Qt5Core)
target_link_libraries(${PROJECT_NAME}  Qt5::Core)

target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)
