
file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)

add_library(imgui ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(imgui glfw3)
target_include_directories(imgui PUBLIC .)