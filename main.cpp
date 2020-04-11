#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <tuple>

#include "shader.h"
#include "skybox.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

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

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Racer3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::vector<float> vertices = icosahedron_vertices(0.5);
    std::vector<unsigned int> indices = icosahedron_indices();

    /* float* vertices = std::get<0> */

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

    Shader shader("shaders/basic.vs", "shaders/basic.fs");
    Shader sky_shader("shaders/skybox.vs", "shaders/skybox.fs");
	  ArrayObject sky = create_skybox();

    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)) {
    	// handle inputs
    	processInput(window);

    	// render
      // ------
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader.use();

      glm::mat4 view;
      view = glm::lookAt(
          glm::vec3(0.0f, 0.0f, 3.0f),
          glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3(0.0f, 1.0f, 0.0f));
      shader.setMat4("view", view);

      glm::mat4 projection;
      projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
      shader.setMat4("projection", projection);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      glBindVertexArray(vertex_arr);
      glDrawElements(GL_TRIANGLES, 3*20, GL_UNSIGNED_INT, (void*)0);
      glBindVertexArray(0);

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      sky_shader.use();
      glBindVertexArray(sky.vao);
      glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);

  
      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
      // -------------------------------------------------------------------------------
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
