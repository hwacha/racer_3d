#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>

#include "cube.h"
#include "shader.h"

unsigned int make_cube() {
  float vertices[] = {
	-1, -1, -1, 0, 1, 0, 0, -1,
	1, -1,  -1, 1, 1, 0, 0, -1,
	1, 1,   -1, 1, 0, 0, 0, -1,
	1, 1,   -1, 1, 0, 0, 0, -1,
	-1, -1, -1, 0, 1, 0, 0, -1,
	-1, 1,  -1, 0, 0, 0, 0, -1,

	-1, -1, 1,  0, 1, 0, 0, 1,
	1, 1, 1,    1, 0, 0, 0, 1,
	1, -1, 1,   1, 1, 0, 0, 1,
	1, 1, 1,    1, 0, 0, 0, 1,
	-1, 1, 1,   0, 0, 0, 0, 1,
	-1, -1, 1,  0, 1, 0, 0, 1,

	// the faces below are overwritten with mirrored coordinates from
	// the above faces
	-1, -1, -1, 0, 0, 0, 0, -1,
	1, -1, -1,  1, 0, 0, 0, -1,
	1, 1, -1,   1, 1, 0, 0, -1,
	1, 1, -1,   1, 1, 0, 0, -1,
	-1, -1, -1, 0, 0, 0, 0, -1,
	-1, 1, -1,  0, 1, 0, 0, -1,

	-1, -1, -1, 0, 0, 0, 0, -1,
	1, -1, -1,  1, 0, 0, 0, -1,
	1, 1, -1,   1, 1, 0, 0, -1,
	1, 1, -1,   1, 1, 0, 0, -1,
	-1, -1, -1, 0, 0, 0, 0, -1,
	-1, 1, -1,  0, 1, 0, 0, -1,

	-1, -1, -1, 0, 0, 0, 0, -1,
	1, -1, -1, 1, 0, 0, 0, -1,
	1, 1, -1, 1, 1, 0, 0, -1,
	1, 1, -1, 1, 1, 0, 0, -1,
	-1, -1, -1, 0, 0, 0, 0, -1,
	-1, 1, -1, 0, 1, 0, 0, -1,

	-1, -1, -1, 0, 0, 0, 0, -1,
	1, -1, -1,  1, 0, 0, 0, -1,
	1, 1, -1,   1, 1, 0, 0, -1,
	1, 1, -1,   1, 1, 0, 0, -1,
	-1, -1, -1, 0, 0, 0, 0, -1,
	-1, 1, -1,  0, 1, 0, 0, -1,
  };

  for(int i = 2; i < 4; i++) {
	  for(int j = 0; j < 6; j++) {
	    float *src = &vertices[(i-2) * 48 + j * 8];
	    float *dst_xz = &vertices[i * 48 + j * 8];
		float *dst_yz = &vertices[(i + 2) * 48 + j * 8];
		dst_xz[3] = src[3];
		dst_xz[4] = src[4];

		dst_xz[0] = src[2];
	    dst_xz[2] = src[0];
	    dst_xz[5] = src[7];
	    dst_xz[7] = src[5];

		dst_yz[3] = src[3];
		dst_yz[4] = src[4];
		
		dst_yz[1] = src[2];
		dst_yz[2] = src[1];
		dst_yz[6] = src[7];
		dst_yz[7] = src[6];
    }
  }

  unsigned int indices[6 * 6];
  for(int i=0; i<36; i++) indices[i] = i;
  
  unsigned int vertex_arr;
  glGenVertexArrays(1, &vertex_arr);
  glBindVertexArray(vertex_arr);

  unsigned int vertex_buf;
  glGenBuffers(1, &vertex_buf);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(1);
  
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int indices_buf;
  glGenBuffers(1, &indices_buf);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
