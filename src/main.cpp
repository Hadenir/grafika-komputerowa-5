#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <imgui.h>
#include <fstream>
#include <sstream>

#include "display/display.hpp"
#include "render/shader/shader.hpp"
#include "render/texture.hpp"
#include "render/camera.hpp"
#include "render/model.hpp"
#include "object/render_object.hpp"
#include "object/scene.hpp"

std::string read_shader(const std::string& path)
{
    std::ifstream file(path);
    if(file.fail())
        throw std::runtime_error("Could not open file!");

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

int main()
{
    Display display("LearnOpenGL", 800, 600);

    std::string vertex_source = read_shader("resources/shaders/phong.vert");
    std::string fragment_source = read_shader("resources/shaders/phong.frag");
    PhongShader shader(vertex_source, fragment_source);

    Camera camera(glm::vec3(0.0f, 40 * 3.0f, 40 * -5.0f), display.get_width(), display.get_height());

    display.set_keyboard_callback([&](int key) { if(key == GLFW_KEY_ESCAPE) display.close(); });
    display.set_mouse_callback([&](float x, float y) {
        if(display.get_mouse_button(GLFW_MOUSE_BUTTON_RIGHT)) camera.rotate(x, -y);
    });
    display.set_scroll_callback([&](float off) { camera.zoom(-off); });
    display.set_resize_callback([&](size_t width, size_t height) { camera.resize_viewport(width, height); });

    Scene scene(camera);

    float delta_time;
    float last_frame = (float) glfwGetTime();
    while(!display.should_close())
    {
        float current_frame = (float) glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        display.begin_frame();

        scene.update(display, delta_time);

        shader.use();
        scene.draw(shader);

        display.end_frame();
    }

    return 0;
}
