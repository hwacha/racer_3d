#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <tuple>

#include "cube.h"
#include "framebuffer.h"
#include "icosahedron.h"

#ifndef INPUT
#define INPUT
#include "input.h"
#endif

#include "model.h"
#include "obstacle.h"

#ifndef PLAYER_H
#define PLAYER_H
#include "player.h"
#endif

#include "shader.h"
#include "skybox.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

unsigned int scr_width = 1280;
unsigned int scr_height = 720;



unsigned int createTexture(char* filename)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;//?
}



int main()
{
    int i;
    int j;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "Racer3D", NULL, NULL);
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

    // World
    std::vector<Obstacle> world_cubes{};

    // Floor
    for ( i = 0 ; i < 10 ; i++ ) {
        float scale = 5.0f;
        Obstacle floor_piece{scale, glm::vec3(2.0f * scale * i, -scale, 0.0f)};
        world_cubes.push_back(floor_piece);
    }
    // obstacles
   	float scale = 1.0f;
   	Obstacle obs_1{scale, glm::vec3(10.0f, 0.5f, 0.0f)};
   	world_cubes.push_back(obs_1);

   	Obstacle obs_2{scale, glm::vec3(20.0f, 0.5f, 3.0f)};
   	world_cubes.push_back(obs_2);

   	Obstacle obs_3{scale, glm::vec3(30.0f, 0.5f, -3.0f)};
   	world_cubes.push_back(obs_3);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int icosahedron_va = make_icosahedron();
    unsigned int cube_va = make_cube();

    Shader icosahedron_shader("shaders/icosahedron.vs", "shaders/icosahedron.fs");
    ///Shader icosahedron_edge_shader("shaders/icosahedron_edge.vs", "shaders/icosahedron_edge.fs");
    Shader obstacle_shader("shaders/obstacle.vs", "shaders/obstacle.fs");
    Shader sky_shader("shaders/skybox.vs", "shaders/skybox.fs");
    Shader level_shader("shaders/level.vs", "shaders/level.fs");
    Shader pass_shader("shaders/shader_pass1.vs", "shaders/shader_pass1.fs");

    ArrayObject sky = create_fullscreen_quad();
    Model test_level("assets/zone.obj");
    unsigned int testTexture = createTexture("assets/checker-map_tho.png");

    glEnable(GL_DEPTH_TEST);

    /* glm::vec3 player_position{ 0,0,0 }; */
    Player player = initial_player();

    FrameBuffer buffer0{scr_width, scr_height};

    while(!glfwWindowShouldClose(window)) {
      buffer0.activate();

    	// handle inputs
      PlayerInputs inputs = poll_inputs(window);
      if (inputs.key_esc_pressed) {
          break;
      }

      step_player(inputs, &player);

    	// render
      // ------
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // view
      glm::mat4 view;
      view = glm::lookAt(
          player.position - 2.0f*player.forward() + glm::vec3(0.0f, 0.5f, 0.0f),
          player.position,
          glm::vec3(0.0f, 1.0f, 0.0f));

      // projection
      glm::mat4 projection;
      projection = glm::perspective(glm::radians(45.0f),
        scr_width / (float) scr_height, 1.f, 1000.0f);

      // icosahedron

      glm::mat4 icosa_roll = glm::rotate(
          glm::mat4(1.0f),
          player.pitch_rads,
          glm::vec3(0.0f, 0.0f, -1.0f));
      glm::mat4 icosa_yaw = glm::rotate(
          glm::mat4(1.0f),
          player.yaw_rads,
          glm::vec3(0.0f, 1.0f, 0.0f));
      glm::mat4 icosa_trans = glm::translate(glm::mat4(1.0f), player.position);
      glm::mat4 icosahedron_model =
          icosa_trans * icosa_yaw * icosa_roll * glm::mat4(1.0f);

        // icosahedron body
      icosahedron_shader.use();
      icosahedron_shader.setMat4("view", view);
      icosahedron_shader.setMat4("projection", projection);
      draw_icosahedron(icosahedron_shader, icosahedron_model, icosahedron_va);

      // icosahedron edges
      // icosahedron_edge_shader.use();

      // icosahedron_edge_shader.setMat4("view", view);
      // icosahedron_edge_shader.setMat4("projection", projection);

      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      // draw_icosahedron(icosahedron_edge_shader, icosahedron_model, icosahedron_va);
      // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // reset


      // floor and obstacles
      level_shader.use();

      level_shader.setMat4("view", view);
      level_shader.setMat4("projection", projection);

      for (auto obs : world_cubes) {
          glm::mat4 id_mat = glm::mat4(1.0f);

          glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(obs.scale));
          glm::mat4 trans_mat = glm::translate(glm::mat4(1.0f), obs.position);

          glm::mat4 obstacle_model = trans_mat * scale_mat * id_mat;

		  glBindTexture(GL_TEXTURE_2D, testTexture);
          draw_cube(level_shader, obstacle_model, cube_va);
      }

      // sky

      sky_shader.use();
      glBindVertexArray(sky.vao);
      glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);

      // level

      //level_shader.use();
      //level_shader.setMat4("model",
      // glm::scale(glm::mat4(1.0f), glm::vec3{ 0.1f, 0.1f, 0.1f }));
      //level_shader.setMat4("view", view);
      //level_shader.setMat4("projection", projection);
      //test_level.Draw(level_shader);


      // unbind the recorded buffer, now render it as a fullscreen quad to the real frame buffer
      buffer0.activate(false);

      renderFramebuffer(pass_shader, buffer0);

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
      // -------------------------------------------------------------------------------
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    scr_width = width;
    scr_height = height;
}
