cmake_minimum_required(VERSION 3.21)
project(grafika_komputerowa_4)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.hpp)

add_executable(grafika_komputerowa_4 ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(grafika_komputerowa_4 glad glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})
