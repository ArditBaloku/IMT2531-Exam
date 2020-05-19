#include "game.h"

float accumulator = 0.0f;
glm::vec3 color = glm::vec3(.6f, .8f, .8f);
float eagleRotation = 180.0f;

void Game::init() {
  shader = Shader("../resources/shaders/vertex.vert", "../resources/shaders/fragment.frag");
  skyboxShader = Shader("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");

  skybox.load();

  level.load("resources/levels/heightmap.png");
  player.load("resources/models/duck.obj", "resources/textures/duck.jpg");
  eagle.load("resources/models/eagle.obj", "resources/textures/eagle.tga");
  deer.load("resources/models/deer.obj", "resources/textures/deer.jpg");
}

void Game::update(float dt, float cycleSpeed) {
  setLighting(dt, cycleSpeed);
}

void Game::processInput(float dt) {
  if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, dt);
  if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, dt);
  if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, dt);
  if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, dt);
  if (keys[GLFW_KEY_T]) view = (view + 1) % 3;
  if (keys[GLFW_KEY_LEFT_SHIFT]) {
    camera.ProcessKeyboard(RUN, dt);
  } else {
    camera.ProcessKeyboard(STOP_RUN, dt);
  }
}

void Game::render() {
  shader.reset();
  setUpTransformations();

  level.draw(shader);
  drawPlayer();

  drawObjects();

  skybox.draw(skyboxShader);
}

void Game::setUpTransformations() {
  auto projection = glm::perspective(glm::radians(camera.Zoom), 16.f / 9.f, 0.01f, 650.f);
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

  skyboxShader.use();
  skyboxShader.setMat4("view", glm::mat4(glm::mat3(lookAt)));
  skyboxShader.setMat4("projection", glm::perspective(glm::radians(50.f), 16.f / 9.f, 0.01f, 650.f));
}

void Game::setLighting(float dt, float cycleSpeed) {
  accumulator += dt * cycleSpeed;

  if (glm::sin(accumulator) < -.2f) accumulator = 0;

  glm::vec3 diffuseStrength = glm::vec3(glm::max(glm::sin(accumulator), .2f));
  if (diffuseStrength.x > .4f && diffuseStrength.x < .5f) {
    color += glm::vec3(.01f, .0f, .0f);
  } else if (diffuseStrength.x > .2f && diffuseStrength.x < .3f) {
    color += glm::vec3(-.01f, .0f, .0f);
  } else if (diffuseStrength.x > .5f) {
    color = glm::vec3(.8f, .8f, .8f);
  } else if (diffuseStrength.x < .2f) {
    color = glm::vec3(.6f, .8f, .8f);
  }

  shader.use();
  shader.setDirLight(
    glm::vec3(.2f, -1.f, glm::sin(accumulator) - glm::cos(accumulator)), 
    glm::vec3(0.1f), 
    diffuseStrength, 
    glm::vec3(.4f), 
    color);
}

void Game::drawPlayer() {
  if (view != FIRST_PERSON) {
    player.draw(camera.Position, 0.06, glm::vec3(-90, 0, 0), shader);
  }
}

void Game::drawObjects() {
  eagleRotation += 1.65f;
  if (eagleRotation > 360) eagleRotation = 0;

  player.draw(glm::vec3(285, 1 + glm::clamp(glm::cos(glfwGetTime()), -.3 ,.2), 240 + glm::sin(glfwGetTime())), 0.02, glm::vec3(-90, 0, 0), shader);
  eagle.draw(glm::vec3(285 + 3*glm::sin(glfwGetTime()), 20 + glm::sin(glfwGetTime()), 240 + 3*glm::cos(glfwGetTime())), .03, glm::vec3(0, eagleRotation, 0), shader);
  deer.draw(glm::vec3(308, 4.2, 201), .02, glm::vec3(-90, 0, 0), shader);
}
