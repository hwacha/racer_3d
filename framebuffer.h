#include "skybox.h"

struct FrameBuffer {
	unsigned int fbo;
	unsigned int texture_color, texture_stencil_depth;

  // unsigned int rbo;

  FrameBuffer(unsigned int width, unsigned int height) {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // create a texture and attach it to the framebuffer
    glGenTextures(1, &texture_color);
    glBindTexture(GL_TEXTURE_2D, texture_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color, 0);
    // depth/stencil buffer
    glGenTextures(1, &texture_stencil_depth);
    glBindTexture(GL_TEXTURE_2D, texture_stencil_depth);
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
      GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
    );
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture_stencil_depth, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		// std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		printf("ERROR::FRAMEBUFFERLL Framebuffer is not complete!\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glGenRenderbuffers(1, &rbo);
  }
  void activate(bool isActive = true) {
    if (isActive)
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    else
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  void todo() {

  }
};

void renderFramebuffer(Shader pass_shader, FrameBuffer buffer0) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    pass_shader.use();
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(pass_shader.ID, "screenTexture"), 0);
    // uniform location 0 gets texture unit 0
    glBindTexture(GL_TEXTURE_2D, buffer0.texture_color);
    ArrayObject full_screen_quad = create_fullscreen_quad();
    glBindVertexArray(full_screen_quad.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
};
