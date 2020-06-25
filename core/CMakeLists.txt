project(editor_core)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)
