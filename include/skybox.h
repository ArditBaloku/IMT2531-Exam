#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "shader.h"
#include <vector>
#include <string>

class Skybox {
  public:
    void load();
    void draw(Shader shader);
  private:
    void loadCubeMap(std::vector<std::string> faces);
    uint32_t m_vbo{};
    uint32_t m_vao{};
    uint32_t m_texture{};
};

#endif