
#include "render.h"

void Render::prepare() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Render::render(RawModel rawModel) {
  glBindVertexArray(rawModel.vaoID);
  int vaoMaxIndex = rawModel.vaoMaxIndex;
  for (int i = 0; i < vaoMaxIndex; ++i) {
    glEnableVertexAttribArray(i);
  }
  glDrawElements(GL_TRIANGLES, rawModel.vertexCounts, GL_UNSIGNED_INT, 0);
  for (int i = 0; i < vaoMaxIndex; ++i) {
    glDisableVertexAttribArray(i);
  }
  glBindVertexArray(0);
}
