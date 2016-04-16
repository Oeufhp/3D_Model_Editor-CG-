#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::mat4 getModelMove();
bool getColorChange();
glm::vec3 * getRay();
bool isLeftClick();
bool isMoveUp();
bool isMoveRight();
bool isKeyUp();
bool isKeyRight();
bool isKeyI();
bool isKeyJ();
bool isKeyK();
bool isKeyL();
#endif