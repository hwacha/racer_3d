#pragma once

struct ArrayObject {
  GLuint vao, array_buf, element_buf;
};

ArrayObject create_fullscreen_quad(float z = 0.99999);
