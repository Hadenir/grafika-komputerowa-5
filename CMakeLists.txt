cmake_minimum_required(VERSION 3.21)
project(grafika_komputerowa_4)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_SHARED_LIBS "" OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS "" OFF)
option(ASSIMP_BUILD_TESTS "" OFF)

add_subdirectory(lib)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.hpp)

add_executable(grafika_komputerowa_4 ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(grafika_komputerowa_4
    glad glfw3 stb_image glm imgui assimp ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES}
)

add_custom_command(TARGET grafika_komputerowa_4 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
    COMMENT "Copying resources directory..."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)