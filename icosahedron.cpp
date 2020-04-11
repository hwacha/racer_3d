#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>

#include "icosahedron.h"
#include "shader.h"

std::vector<float> icosahedron_vertices(float radius) {
    int i;
    float elev_angle = atan(0.5);
    float rot_angle = 72.0 * 3.1415 / 180.0;

    std::vector<float> vertices(12*3);
    // top
    vertices[0*3 + 0] = 0.0;
    vertices[0*3 + 1] = 0.0;
    vertices[0*3 + 2] = radius;
    // upper ring
    for ( i = 0 ; i < 5 ; i++ ) {
        vertices[(i+1)*3 + 0] = radius * cos(i*rot_angle) * cos(elev_angle);
        vertices[(i+1)*3 + 1] = radius * sin(i*rot_angle) * cos(elev_angle);
        vertices[(i+1)*3 + 2] = radius * sin(elev_angle);
    }
    // lower ring
    for ( i = 0 ; i < 5 ; i++ ) {
        vertices[(i+6)*3 + 0] = radius * cos((i+0.5)*rot_angle) * cos(elev_angle);
        vertices[(i+6)*3 + 1] = radius * sin((i+0.5)*rot_angle) * cos(elev_angle);
        vertices[(i+6)*3 + 2] = radius * -sin(elev_angle);
    }
    // bottom
    vertices[11*3 + 0] = 0.0;
    vertices[11*3 + 1] = 0.0;
    vertices[11*3 + 2] = -radius;

    return vertices;
}

std::vector<unsigned int> icosahedron_indices() {
    int i;
    unsigned int upper_ring[] = {1, 2, 3, 4, 5};
    unsigned int lower_ring[] = {6, 7, 8, 9, 10};
    std::vector<unsigned int> indices(20*3);
    // row 1
    for ( i = 0 ; i < 5 ; i++ ) {
        indices[i*3 + 0] = 0;
        indices[i*3 + 1] = upper_ring[i];
        indices[i*3 + 2] = upper_ring[(i+1)%5];
    }
    // row 2
    for ( i = 0 ; i < 5 ; i++ ) {
        indices[(i+5)*3 + 0] = i+6;
        indices[(i+5)*3 + 1] = upper_ring[i];
        indices[(i+5)*3 + 2] = upper_ring[(i+1)%5];
    }
    // row 3
    for ( i = 0 ; i < 5 ; i++ ) {
        indices[(i+10)*3 + 0] = lower_ring[i];
        indices[(i+10)*3 + 1] = lower_ring[(i+1)%5];
        indices[(i+10)*3 + 2] = 1 + ((i+1)%5);
    }
    // row 4
    for ( i = 0 ; i < 5 ; i++ ) {
        indices[(i+15)*3 + 0] = lower_ring[i];
        indices[(i+15)*3 + 1] = lower_ring[(i+1)%5];
        indices[(i+15)*3 + 2] = 11;
    }

    return indices;
}

unsigned int make_icosahedron() {
    std::vector<float> vertices = icosahedron_vertices(0.5);
    std::vector<unsigned int> indices = icosahedron_indices();

    unsigned int vertex_arr;
    glGenVertexArrays(1, &vertex_arr);
    glBindVertexArray(vertex_arr);

    unsigned int vertex_buf;
    glGenBuffers(1, &vertex_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*12, vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int indices_buf;
    glGenBuffers(1, &indices_buf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*3*20, indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    return vertex_arr;
}

void draw_icosahedron(
    Shader shader,
    glm::mat4 transform,
    unsigned int icosahedron_va)
{
    shader.setMat4("model", transform);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(icosahedron_va);
    glDrawElements(GL_TRIANGLES, 3*20, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // reset
}
