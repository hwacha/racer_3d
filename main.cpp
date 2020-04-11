#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

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

    float vertices[] = {
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2
    };

    unsigned int vertex_arr;
    glGenVertexArrays(1, &vertex_arr);
    glBindVertexArray(vertex_arr);

    unsigned int vertex_buf;
    glGenBuffers(1, &vertex_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int indices_buf;
    glGenBuffers(1, &indices_buf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    Shader shader("shaders/basic.vs", "shaders/basic.fs");

    while(!glfwWindowShouldClose(window)) {
    	// handle inputs
    	processInput(window);

    	// render
      // ------
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader.use();

      glBindVertexArray(vertex_arr);
      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);
      glBindVertexArray(0);

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
