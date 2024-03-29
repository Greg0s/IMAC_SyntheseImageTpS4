#include <GL/glew.h>
#include <cstddef>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/glm.hpp>
#include <iostream>

using namespace glimac;

class Vertex2DColor {
public:
  glm::vec2 m_position;
  glm::vec3 m_color;

  Vertex2DColor(glm::vec2 position = glm::vec2(0.f, 0.f),
                glm::vec3 color = glm::vec3(0.f, 0.f, 0.f))
      : m_position(position), m_color(color){};
};

int main(int argc, char **argv) {
  // Initialize SDL and open a window
  SDLWindowManager windowManager(800, 600, "GLImac");

  // Initialize glew for OpenGL3+ support
  GLenum glewInitError = glewInit();
  if (GLEW_OK != glewInitError) {
    std::cerr << glewGetErrorString(glewInitError) << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

  /*LOAD SHADERS*/
  FilePath applicationPath(argv[0]);
  Program program =
      loadProgram(applicationPath.dirPath() + "shaders/color2D.vs.glsl",
                  applicationPath.dirPath() + "shaders/color2D.fs.glsl");
  program.use();
  /*****************/

  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/

  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  Vertex2DColor vertices[] = {
      Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(1, 0, 0)),
      Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0)),
      Vertex2DColor(glm::vec2(0, 0.5), glm::vec3(0, 0, 1))};

  glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DColor), vertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint vao;
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  const GLuint VERTEX_ATTR_POSITION = 0;
  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

  const GLuint VERTEX_ATTR_COLOR = 1;
  glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex2DColor),
                        (const GLvoid *)(offsetof(Vertex2DColor, m_position)));

  glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex2DColor),
                        (const GLvoid *)(offsetof(Vertex2DColor, m_color)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  /*********************************
   * END OF THE INITIALIZATION CODE
   *********************************/

  // Application loop:
  bool done = false;
  while (!done) {
    // Event loop:
    SDL_Event e;
    while (windowManager.pollEvent(e)) {
      if (e.type == SDL_QUIT) {
        done = true; // Leave the loop after this iteration
      }
    }

    /*********************************
     * HERE SHOULD COME THE RENDERING CODE
     *********************************/

    glClearColor(1.f, 0.5f, 0.5f, 1.f); // background color

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    /*********************************
     * END OF THE RENDERING CODE
     *********************************/

    // Update the display
    windowManager.swapBuffers();
  }

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  return EXIT_SUCCESS;
}
