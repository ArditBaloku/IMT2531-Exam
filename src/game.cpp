#include "game.h"

void Game::init() {
  shader = Shader("../resources/shaders/vertex.vert", "../resources/shaders/fragment.frag");

  level.load("resources/levels/Randsf_HeightMap.png");
  player.load("resources/models/duck.obj", "resources/textures/duck.jpg");

}

void Game::update(float dt) {
  
}

void Game::processInput(float dt) {
  if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, dt);
  if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, dt);
  if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, dt);
  if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, dt);
  if (keys[GLFW_KEY_M]) view = (view + 1) % 3;
  if (keys[GLFW_KEY_LEFT_SHIFT]) {
    camera.ProcessKeyboard(RUN, dt);
  } else {
    camera.ProcessKeyboard(STOP_RUN, dt);
  }
}

void Game::render() {
  shader.reset();
  setLighting();
  setUpTransformations();

  level.draw(shader);
  drawPlayer();
}

void Game::setUpTransformations() {
  auto projection = glm::perspective(glm::radians(50.f), 16.f / 9.f, 0.01f, 650.f);
  auto lookAt = glm::mat4(1.0f);

  switch (view) {
    case FIRST_PERSON:
      lookAt = camera.GetFirstPersonView();
      break;
    case THIRD_PERSON:
      lookAt = camera.GetThirdPersonView();
      break;
    case BIRD_VIEW:
      lookAt = camera.Get2DView();
      break;
  }

  shader.setMat4("view", lookAt);
  shader.setMat4("projection", projection);
}

void Game::setLighting() {
  shader.setDirLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(0.5f));
}

void Game::drawPlayer() {
  if (view != FIRST_PERSON) {
    player.draw(camera.Position, 0.06, -90, shader);
  }
}
