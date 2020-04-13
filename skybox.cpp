#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "skybox.h"

ArrayObject create_fullscreen_quad(float z) {
  float skybox_vertices[] = {
   -1 , -1,  z,
   -1,   1,  z,
    1,  -1,  z,
    1,   1,  z,

   0, 0,
   0, 1,
   1, 0,
   1, 1,
  };

  GLuint skybox_indices[] = { 0, 1, 2, 1, 2, 3, };
  ArrayObject sky;
  glGenVertexArrays(1, &sky.vao);
  glGenBuffers(1, &sky.array_buf);
  glGenBuffers(1, &sky.element_buf);

  glBindVertexArray(sky.vao);
  glBindBuffer(GL_ARRAY_BUFFER, sky.array_buf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), skybox_vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(float) * 12));
  glEnableVertexAttribArray(2);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky.element_buf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skybox_indices), skybox_indices, GL_STATIC_DRAW);
  glBindVertexArray(0);
  return sky;
}
