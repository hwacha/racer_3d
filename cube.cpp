#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>

#include "cube.h"
#include "shader.h"

unsigned int make_cube() {
    float vertices[] = {
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,
        1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
    };

    unsigned int indices[] = {
        0,1,2,
        0,2,3,
        0,4,1,
        1,4,5,
        1,5,2,
        2,5,6,
        2,6,3,
        3,6,7,
        3,7,0,
        0,7,4,
        4,6,5,
        4,7,6
    };

    unsigned int vertex_arr;
    glGenVertexArrays(1, &vertex_arr);
    glBindVertexArray(vertex_arr);

    unsigned int vertex_buf;
    glGenBuffers(1, &vertex_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*8, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int indices_buf;
    glGenBuffers(1, &indices_buf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*3*12, indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    return vertex_arr;
}

void draw_cube(
    Shader shader,
    glm::mat4 transform,
    unsigned int cube_va)
{
    shader.setMat4("model", transform);

    glBindVertexArray(cube_va);
    glDrawElements(GL_TRIANGLES, 3*12, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
