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

    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), display.get_width(), display.get_height());

    display.set_keyboard_callback([&](int key) { if(key == GLFW_KEY_ESCAPE) display.close(); });
    display.set_mouse_callback([&](float x, float y) { camera.rotate(x, -y); });
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

        if(display.get_key(GLFW_KEY_W))
            camera.move(Direction::Forward, delta_time);
        if(display.get_key(GLFW_KEY_S))
            camera.move(Direction::Backward, delta_time);
        if(display.get_key(GLFW_KEY_D))
            camera.move(Direction::Right, delta_time);
        if(display.get_key(GLFW_KEY_A))
            camera.move(Direction::Left, delta_time);
        if(display.get_key(GLFW_KEY_SPACE))
            camera.move(Direction::Up, delta_time);
        if(display.get_key(GLFW_KEY_LEFT_SHIFT))
            camera.move(Direction::Down, delta_time);

        scene.update(delta_time);

        shader.use();
        scene.draw(shader);

        // ImGui::ShowDemoWindow();

        display.end_frame();
    }

    return 0;
}
