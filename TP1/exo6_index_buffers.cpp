#include <GL/glew.h>
#include <cstddef>
#include <cstdint>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/glm.hpp>
#include <iostream>
#include <vector>

using namespace glimac;

class Vertex3DColor {
public:
  glm::vec3 m_position;
  glm::vec3 m_color;

  Vertex3DColor(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                glm::vec3 color = glm::vec3(0.f, 0.f, 0.f))
      : m_position(position), m_color(color){};
};

int main(int argc, char **argv) {
  // Initialize SDL and open a window
  SDLWindowManager windowManager(800, 800, "GLImac");

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
      loadProgram(applicationPath.dirPath() + "shaders/triangle.vs.glsl",
                  applicationPath.dirPath() + "shaders/triangle.fs.glsl");
  program.use();
  /*****************/

  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/

  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  const GLuint NB_TRIANGLES = 50;
  const glm::vec3 color = {0.f, 0.f, 0.f};
  const GLfloat r = 0.5;
  GLfloat angle = 0;
  glm::vec3 coords = {r * glm::cos(angle), r * glm::sin(angle), 0};

  /*********************************
   *VERTICES CREATION
   *********************************/
  std::vector<Vertex3DColor> vertices;
  std::vector<uint32_t> indices;

  vertices.push_back(Vertex3DColor({0.f, 0.f, 0.f}, color));

  for (uint i = 0; i <= NB_TRIANGLES; i++) {
    indices.push_back(0);
    indices.push_back(i);
    indices.push_back(i + 1);
    vertices.push_back(Vertex3DColor(coords, color));
    angle += 2 * glm::pi<float>() / NB_TRIANGLES;
    coords = {r * glm::cos(angle), r * glm::sin(angle), 0};
  }
  // indices.push_back(2);
  //  indices.push_back(NB_TRIANGLES);

  glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(Vertex3DColor),
               &vertices.front(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint ibo;
  glGenBuffers(1, &ibo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t),
               indices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLuint vao;
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  const GLuint VERTEX_ATTR_POSITION = 3;
  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

  const GLuint VERTEX_ATTR_COLOR = 8;
  glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex3DColor),
                        (const GLvoid *)(offsetof(Vertex3DColor, m_position)));

  glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex3DColor),
                        (const GLvoid *)(offsetof(Vertex3DColor, m_color)));

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
    glDrawElements(GL_TRIANGLES, (NB_TRIANGLES + 1) * 3, GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_TRIANGLES, NB_TRIANGLES * 3, GL_UNSIGNED_INT, 3);

    // glDrawArrays(GL_TRIANGLES, 3, vertices.size());

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
