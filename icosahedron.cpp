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
    std::vector<float> unique_vertices = icosahedron_vertices(0.5);
    std::vector<unsigned int> indices = icosahedron_indices();

    int attribStride = 9;
    std::vector<float> vertices = std::vector<float>(attribStride*3*20);

    int triangle = 0;
    int in_vertex = 0;
    int out_vertex = 0;
    int index = 0;
    int i = 0;
    int v = 0;
    float acc_vec[] = { 0.0f, 0.0f, 0.0f };
    int triangle_no = 0;

    // For each face (triangle) ...
    for ( triangle = 0 ; triangle < 20 ; triangle++ ) {
        acc_vec[0] = 0.0; acc_vec[1] = 0.0; acc_vec[2] = 0.0; // reset accumulator for face normal

        // ... and for each of its vertices ...
        for ( in_vertex = 0 ; in_vertex < 3 ; in_vertex++ ) {
            // ... find its index in the unique vertices ...
            out_vertex = triangle*3 + in_vertex;
            index = indices[out_vertex];

            // ... write out the vertex coordinates ...
            for ( i = 0 ; i < 3 ; i++ ) {
                vertices[out_vertex*attribStride + i] =
                    unique_vertices[3*index + i];
                acc_vec[i] +=
                    unique_vertices[3*index + i];
            }
            // ... and write out the barycentric coordinates ...
            for ( i = 0 ; i < 3 ; i++ ) {
                float bary;
                if (i == in_vertex) {
                    bary = 1.0;
                } else {
                    bary = 0.0;
                }
                vertices[out_vertex*attribStride + 3 + i] = bary;
            }
        }
        // ... and write out face normals
        glm::vec3 face_normal = glm::normalize(
            glm::vec3(acc_vec[0], acc_vec[1], acc_vec[2])
        );
        for ( v = 0 ; v < 3 ; v++ ) {
            out_vertex = 3*triangle + v;
            for ( i = 0 ; i < 3 ; i ++ ){
                vertices[out_vertex*attribStride + 6 + i] = face_normal[i];
            }
        }
        triangle_no++;
    }

    unsigned int vertex_arr;
    glGenVertexArrays(1, &vertex_arr);
    glBindVertexArray(vertex_arr);

    unsigned int vertex_buf;
    glGenBuffers(1, &vertex_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size()*sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return vertex_arr;
}

void draw_icosahedron(
    Shader shader,
    glm::mat4 transform,
    unsigned int icosahedron_va)
{
    shader.setMat4("model", transform);
    shader.setFloat("t", (float)glfwGetTime());
    glBindVertexArray(icosahedron_va);
    glDrawArrays(GL_TRIANGLES, 0, 3*20);
    glBindVertexArray(0);
}
