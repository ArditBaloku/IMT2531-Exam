#include "level.h"
#include <stb_image.h>
#include <gfx.h>
#include <glm/gtc/matrix_transform.hpp>

void Level::load(const std::string& filepath) {
  m_texture.load_texture("resources/textures/colorscale.png");

  // load the heightmap
  int x, y;
  stbi_set_flip_vertically_on_load(true);
  auto* pixels = stbi_load(filepath.c_str(), &x, &y, nullptr, STBI_rgb_alpha);

  std::vector<float> tmp;

  // for each pixel in the heightmap, translate it to a value between 0.0 and 1.0 and store it in the heights array
  for (int i = 0; i < x; i++) {
    tmp.clear();
    for (int j = 0; j < y; j++) {
      unsigned bytePerPixel = 4;
      unsigned char* pixelOffset = pixels + (i + y * j) * bytePerPixel;
      unsigned char r = pixelOffset[0];
      tmp.push_back((float)r / 255.0);

    }
    heights.push_back(tmp);
  }

  stbi_image_free(pixels);

  GFX_INFO("Loaded heightmap");

  float size = 15.f;
  int vertexCount = heights.size();
  std::vector<Vertex> out_vertices{};
  std::vector<unsigned> out_indices{};

  std::vector<glm::vec3> tmp2;
  // create a vertex for each point and assign normals and texcoords to it
  for (int i = 0; i < vertexCount; i++) {
    for (int j = 0; j < vertexCount; j++) {
      out_vertices.push_back({});
      
      auto &vertex = out_vertices.back();
      vertex.position = {(float)i / (float)(vertexCount - 1) * size,
                         heights[i][j],
                         (float)j / ((float)vertexCount - 1) * size};

      vertex.normals = {getNormal(i, j)};

      vertex.texcoord = {heights[i][j], 0};

      if (heights[i][j] > 0.18 && heights[i][j] < 0.26) {
        tmp2.push_back(vertex.position);
      }
    }
  }

  for (int i = 0; i < tmp2.size(); i += 300) {
    treeCoords.push_back(tmp2[i]);
  }

  int numStripsRequired = vertexCount - 1;
  int numDegensRequired = 2 * (numStripsRequired - 1);
  int verticesPerStrip = 2 * vertexCount;
  
  for (int y = 0; y < vertexCount - 1; y++) {      
      if (y > 0) {
          // Degenerate begin: repeat first vertex
          out_indices.push_back((unsigned) (y * vertexCount));
      }
  
      for (int x = 0; x < vertexCount; x++) {
          // One part of the strip
          out_indices.push_back((unsigned) ((y * vertexCount) + x));
          out_indices.push_back((unsigned) (((y + 1) * vertexCount) + x));
      }
  
      if (y < vertexCount - 2) {
          // Degenerate end: repeat last vertex
          out_indices.push_back((unsigned) (((y + 1) * vertexCount) + (vertexCount - 1)));
      }
  }

  m_index_count = out_indices.size();
  
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * out_vertices.size(), out_vertices.data(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * out_indices.size(), out_indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normals));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoord));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  GFX_INFO("Loaded terrain.");

  tree.load("resources/models/pine.obj", "resources/textures/pine.jpg");
}

// calculate the normal of a vertex with the finite difference method
glm::vec3 Level::getNormal(int x, int z) {
  float heightL = x != 0 ? heights[x-1][z] : 0;
  float heightR = x != heights.size() - 1 ? heights[x+1][z] : 0;
  float heightU = z != heights.size() - 1 ? heights[x][z+1] : 0;
  float heightD = z != 0 ? heights[x][z-1] : 0;

  return glm::normalize(glm::vec3(heightL - heightR, 2.f, heightD - heightU));
}

void Level::draw(Shader shader) {
  shader.use();
  auto model = glm::translate(glm::mat4(1.f), glm::vec3(0.0f));
  model = glm::scale(model, glm::vec3(30.f));
  model = glm::rotate(model, glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f));
  auto normal_matrix = glm::mat3(model);

  shader.setMat4("model", model);
  shader.setMat3("normal_matrix", normal_matrix);

  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  m_texture.bind(0);
  glDrawElements(GL_TRIANGLE_STRIP, m_index_count, GL_UNSIGNED_INT, nullptr);

  // set everything to default
  glBindVertexArray(0);
  glActiveTexture(GL_TEXTURE0);

  for (int i = 0; i < treeCoords.size(); i++) {
    tree.draw(treeCoords[i] * glm::vec3(30), .003f, -90, shader);
  }
}