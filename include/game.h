#pragma once
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"
#include "level.h"
#include "camera.h"

enum CameraState {
  FIRST_PERSON,
  THIRD_PERSON,
  BIRD_VIEW
};

class Game {
  public:
    bool keys[1024];
    Camera camera = Camera(glm::vec3(-1.0f, 0.0f, 13.0f));
    void init();
    void update(float dt);
    void processInput(float dt);
    void render();
  private:
    int view = 0;
    Model player;
    Level level;
    Shader shader;
    void setLighting();
    void setUpTransformations();
    void drawPlayer();
};

#endif