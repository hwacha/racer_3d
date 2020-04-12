#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <tuple>

#include "prism.h"
#include "effect.h"
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

bool collides_with(Player *player, Obstacle *obstacle) {
  float player_radius = 0.5f;

  float rel_x = max(0.0f, abs(player->position.x - obstacle->position.x) - obstacle->scale.x);
  float rel_z = max(0.0f, abs(player->position.z - obstacle->position.z) - obstacle->scale.z);
  return (rel_x*rel_x + rel_z*rel_z <= player_radius*player_radius);
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
    std::vector<Obstacle> world_prisms{};

    // Floor
    Obstacle floor_1{0, -1, glm::vec3(5.0f, 0.5f, 50.0f), glm::vec3(0.0f, -0.5f, 50.0f)};
    world_prisms.push_back(floor_1);

    Obstacle floor_2{0, -1, glm::vec3(50.0f, 0.5f, 5.0f), glm::vec3(50.0f, -0.5f, 100.0f)};
    world_prisms.push_back(floor_2);

    Obstacle floor_3{0, -1, glm::vec3(5.0f, 0.5f, 50.0f), glm::vec3(100.0f, -0.5f, 50.0f)};
    world_prisms.push_back(floor_3);

    Obstacle floor_4{0, -1, glm::vec3(50.0f, 0.5f, 5.0f), glm::vec3(50.0f, -0.5f, 0.0f)};
    world_prisms.push_back(floor_4);

    // obstacles
    float obs_scale = 1.0f;
     Obstacle obs_1{1, -1, glm::vec3(obs_scale), glm::vec3(0.0f, 0.5f, 10.0f)};
     world_prisms.push_back(obs_1);

     Obstacle obs_2{1, -1, glm::vec3(obs_scale), glm::vec3(3.0f, 0.5f, 20.0f)};
     world_prisms.push_back(obs_2);

     Obstacle obs_3{1, -1, glm::vec3(obs_scale), glm::vec3(-3.0f, 0.5f, 30.0f)};
     world_prisms.push_back(obs_3);

     // checkpoints
     unsigned int num_checkpoints = 4;
     Obstacle lap{2, 0, glm::vec3(5.0f, obs_scale, obs_scale), glm::vec3(0.5f, 1.0f, 5.0f)};
     world_prisms.push_back(lap);

     Obstacle checkpoint_1{2, 1, glm::vec3(obs_scale, obs_scale, 5.0f), glm::vec3(5.0f, 1.0f, 100.0f)};
     world_prisms.push_back(checkpoint_1);

     Obstacle checkpoint_2{2, 2, glm::vec3(5.0f, obs_scale, obs_scale), glm::vec3(100.0f, 1.0f, 94.0f)};
     world_prisms.push_back(checkpoint_2);

     Obstacle checkpoint_3{2, 3, glm::vec3(obs_scale, obs_scale, 5.0f), glm::vec3(94.0f, 1.0f, 0.0f)};
     world_prisms.push_back(checkpoint_3);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int icosahedron_va = make_icosahedron();
    unsigned int prism_va = make_prism();

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
    vector<Player> players{};
    players.push_back(initial_player(1, glm::vec2(1.0f, 0.0f)));
    players.push_back(initial_player(2, glm::vec2(-1.0f, 0.0f)));

    FrameBuffer buffer0{scr_width, scr_height};

    int current_checkpoint = 0;
    int current_lap = 0;

	float yvel = 0;
	float grav = -0.01;

    vector<int> player_laps{};
    player_laps.push_back(1);
    player_laps.push_back(1);

    // 1 if player 1 wins, 2 if player 2 wins, etc.
    int winning_player = 0;

	EffectSystem fx_system {};

    // -------------------------------------------------------------------------------------------------------------------------
    // -------------------------------------------------------------------------------------------------------------------------
    // RENDER LOOP
    // -------------------------------------------------------------------------------------------------------------------------
    // -------------------------------------------------------------------------------------------------------------------------

    bool has_logged_victory_message = false;
    while(!glfwWindowShouldClose(window)) {
      buffer0.activate();

	  fx_system.Update(1);

      // handle inputs
      PlayerInputs inputs = poll_inputs(window);
      if (inputs.key_esc_pressed) {
          break;
      }

      if (winning_player) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if (!has_logged_victory_message) {
			for (auto player : players) {
				if (winning_player & (1 << player.id)) {
					std::cout << "Player" << player.id << "wins!" << std::endl;
        		}
        	}
        	has_logged_victory_message = true;    	
        }

        // victory screen
        continue;
      }

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	  glClear(GL_COLOR_BUFFER_BIT);

      for (auto &player : players) {

	      glm::vec3 old_position = glm::vec3(player.position.x, player.position.y, player.position.z);

		  yvel+= grav;
		  player.position.y+= yvel;

		  step_player(inputs, &player);

		  // check if dieded from falling too far
		  if(player.position.y < -5)
		  {
			  // player pos = last checkpoint pos
			  glm::vec3 lastcheckpointpos;
			  for (auto obstacle : world_prisms) {
				  if(obstacle.checkpoint_place==current_checkpoint){
					  lastcheckpointpos = obstacle.position;
					  break;
				  }
			  }

			  player.position = lastcheckpointpos;

		  }

	      // check for collisions.
	      for (auto obstacle : world_prisms) {
	        if (collides_with(&player, &obstacle)) {
	          // normal obstacle.
	          if (obstacle.collision_type == 1) {

	            // TODO: check for collision with player
	             player.position = old_position;
	             player.speed *= -1.0f;
				 fx_system.create_collision((void *)&player, player.position);
	          }

			  // check if on floor
	          if (obstacle.collision_type == 0) {
				  if(yvel<0 && player.position.y<0.5 && player.position.y>-3*grav){
					  yvel = 0;
					  player.position.y = 0.5;
				  }
			  }

	          // checkpoint.
	          if (obstacle.collision_type == 2) {
	            if ((current_checkpoint + 1) % num_checkpoints == obstacle.checkpoint_place) {
	          	  current_checkpoint = (current_checkpoint + 1) % num_checkpoints;
	          	  // we check for a lap, but we only want to do so if we're inside this condition,
	          	  // because that means the last checkpoint was the last one, not when we started at 0.

		          if (current_checkpoint == 0) {
		            // assume one lap for now, otherwise we can increment laps and check max laps, etc.
		            player_laps[player.id - 1]++;
		            std::cout << "Player " << player.id << " is now on lap " << player_laps[player.id - 1] << std::endl;
		            if (player_laps[player.id - 1] >= 4) {
		              winning_player |= 1 << player.id;
		            }
		          }
	            }
	          }
        	}
      	  }

      	  if (player.id == 1) {
			glViewport(0, 0, scr_width / 2, scr_height);
      	  } else {
      	  	glViewport(scr_width / 2, 0, scr_width / 2, scr_height);
      	  }
      	  

      	  // render
      	  // ------
	      glClear(GL_DEPTH_BUFFER_BIT);

	      // view
	      glm::vec3 camera_pos =
	          player.position - 3.0f*player.forward() + glm::vec3(0.0f, 0.75f, 0.0f);
	      glm::mat4 view = glm::lookAt(
	          camera_pos,
	          player.position + glm::vec3(0.0f, 0.5f, 0.0f), // top of icosahedron
	          glm::vec3(0.0f, 1.0f, 0.0f)
	          );

	      // projection
	      glm::mat4 projection;
	      projection = glm::perspective(glm::radians(45.0f),
	        scr_width / (2 * (float) scr_height), 1.f, 1000.0f);

	      // icosahedron
	      for (auto each_player : players) {
	      	glm::mat4 icosa_rotate = glm::mat4_cast(each_player.orientation);
		    glm::mat4 icosa_trans = glm::translate(glm::mat4(1.0f), each_player.position);
		    glm::mat4 icosahedron_model = icosa_trans * icosa_rotate;

		    icosahedron_shader.use();
		    icosahedron_shader.setInt("player_id", each_player.id);
		    icosahedron_shader.setVec3("cameraPos", camera_pos);
		    icosahedron_shader.setMat4("view", view);
		    icosahedron_shader.setMat4("projection", projection);
	      	draw_icosahedron(icosahedron_shader, icosahedron_model, icosahedron_va);
	      }

	      // floor and obstacles
	      level_shader.use();

	      level_shader.setMat4("view", view);
	      level_shader.setMat4("projection", projection);

	      for (auto obs : world_prisms) {
	          glm::mat4 id_mat = glm::mat4(1.0f);

	          glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(obs.scale));
	          glm::mat4 trans_mat = glm::translate(glm::mat4(1.0f), obs.position);

	          glm::mat4 obstacle_model = trans_mat * scale_mat * id_mat;

		      glBindTexture(GL_TEXTURE_2D, testTexture);

		      if (!(obs.collision_type == 2)) {
		        draw_prism(level_shader, obstacle_model, prism_va);
		      }
	      }

	      // sky

	      sky_shader.use();
	      glBindVertexArray(sky.vao);
	      glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);


          // effects
          sky_shader.use();
          fx_system.view = view;
          fx_system.proj = projection;
          fx_system.Draw();
 
		  // level

	      //level_shader.use();
	      //level_shader.setMat4("model",
	      // glm::scale(glm::mat4(1.0f), glm::vec3{ 0.1f, 0.1f, 0.1f }));
	      //level_shader.setMat4("view", view);
	      //level_shader.setMat4("projection", projection);
	      //test_level.Draw(level_shader);
      }


      // unbind the recorded buffer, now render it as a fullscreen quad to the real frame buffer
      glViewport(0, 0, scr_width, scr_height);
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
