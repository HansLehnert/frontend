#include "Background.hpp"

#include "glm/glm.hpp"


Background::Background(std::string instance_name) :
        GraphicObject(instance_name)
{
    // Default colors
    background_color = glm::vec3(0, 0.1, 0.2);
    accent_color = glm::vec3(0.2, 0.6, 0.7);

    // Generate and buffer model data
    glGenBuffers(1, &model_buffer);

    fit(1, 1, 1);

    // Load program
    program = Program::getProgram("background");
    background_color_loc = program->uniformLocation("background_color");
    accent_color_loc = program->uniformLocation("accent_color");
    offset_loc = program->uniformLocation("offset");
}


void Background::render() {
    glDepthMask(GL_FALSE);

    glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glUseProgram(program->getId());

    glUniformMatrix4fv(
        program->uniformLocation("world_matrix"),
        1,
        GL_FALSE,
        (GLfloat*)&world_matrix
    );
    glUniformMatrix4fv(
        program->uniformLocation("model_matrix"),
        1,
        GL_FALSE,
        (GLfloat*)&model_matrix
    );
    glUniform3fv(background_color_loc, 1, (GLfloat*)&background_color);

    // Draw plane
    glUniform3fv(accent_color_loc, 1, (GLfloat*)&background_color);
    glUniform3f(offset_loc, 0, 0, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // Draw grid lines
    glUniform3fv(accent_color_loc, 1, (GLfloat*)&accent_color);
    glUniform3fv(offset_loc, 1, (GLfloat*)&scroll_offset);
    glDrawArrays(GL_LINES, 4, model_size - 4);

    glDepthMask(GL_TRUE);
}


void Background::updateSelf() {
    time++;
    scroll_offset.x = 2 * spacing.x * sin(time * 0.02);
    scroll_offset.y = 2 * spacing.y * cos(time * 0.02);
    scroll_offset.x = remainder(scroll_offset.x, spacing.x) + spacing.x / 2;
    scroll_offset.y = remainder(scroll_offset.y, spacing.y) + spacing.y / 2;
    //position.z = remainder(sin(time * 0.011), 1 / 4.0);

    /*color.x = 0.5 * (1 + sin(time * 0.023));
    color.y = 0.5 * (1 + sin(time * 0.023 + 2));
    color.z = 0.5 * (1 + sin(time * 0.023 + 4));*/
}


void Background::fit(float width, float height, float grid_spacing) {
    scale.x = width;
    scale.y = height;

    spacing.x = grid_spacing / width;
    spacing.y = grid_spacing / height;

    std::vector<glm::vec4> model_data;

    // Back plane
    model_data.push_back(glm::vec4(0, 0, 0, 1));
    model_data.push_back(glm::vec4(1, 0, 0, 1));
    model_data.push_back(glm::vec4(1, 1, 0, 1));
    model_data.push_back(glm::vec4(0, 1, 0, 1));

    // Grid lines
    int lines_x = ceil(1 / spacing.x);
    int lines_y = ceil(1 / spacing.y);

    for (int i = 0; i < lines_x; i++) {
        model_data.push_back({i * spacing.x, -spacing.y, 0, 1});
        model_data.push_back({i * spacing.x, 1,          0, 1});
    }

    for (int i = 0; i < lines_y; i++) {
        model_data.push_back({-spacing.x, i * spacing.y, 0, 1});
        model_data.push_back({1,          i * spacing.y, 0, 1});
    }

    // for (int i = -div; i <= div; i++) {
    //     for (int j = 0; j < 4; j++) {
    //         model_data.push_back(glm::vec4(-2, 2.0 * i / div, j / 4.0, 1));
    //         model_data.push_back(glm::vec4(2, 2.0 * i / div, j / 4.0, 1));
    //         model_data.push_back(glm::vec4(2.0 * i / div, -2, j / 4.0, 1));
    //         model_data.push_back(glm::vec4(2.0 * i / div, 2, j / 4.0, 1));
    //     }
    // }

    // for (int i = -div; i <= div; i++) {
    //     for (int j = -div; j <= div; j++) {
    //         model_data.push_back(glm::vec4(2.0 * i / div, 4.0 * j / div, 0, 1));
    //         model_data.push_back(glm::vec4(2.0 * i / div, 4.0 * j / div, 1, 1));
    //     }
    // }

    model_size = model_data.size();

    glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        model_size * sizeof(glm::vec4),
        model_data.data(),
        GL_STATIC_DRAW
    );
}
