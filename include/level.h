#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <tuple>
#include <map>
#include <vector>

#include "model.h"


class Level {
  public:
    void load(const std::string& filepath);
    void draw(Shader shader);
  private:
    std::vector<std::vector<float>> heights;
    struct Vertex {
      glm::vec3 position{};
      glm::vec3 normals{};
      glm::vec2 texcoord{};
    };
    glm::vec3 getNormal(int x, int y);
    uint32_t m_vbo{};
    uint32_t m_ebo{};
    uint32_t m_vao{};
    uint32_t m_index_count;
    Texture m_texture{};
    Model tree;
    Model lamp;
    std::vector<glm::vec3> treeCoords;
};

#endif