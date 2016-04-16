// Include GLFW
#include <glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"
#include <iostream>
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 modelMove = mat4(1.0f);
vec3 ray[2];
glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}
glm::mat4 getModelMove() {
	return modelMove;
}
vec3 * getRay() {
	return ray;
}
static bool colorChange = false;
static bool leftClick = false;
bool getColorChange() {
	return colorChange;
}
bool isLeftClick() {
	return leftClick;
}
static bool moveRight,moveUp;
bool isMoveRight() {
	return moveRight;
}
bool isMoveUp() {
	return moveUp;
}
bool static keyUp, keyRight;
bool isKeyUp(){
	return keyUp;
}
bool isKeyRight() {
	return keyRight;
}
bool static keyI=false, keyK=false, keyJ=false, keyL=false;
bool isKeyI() {
	return keyI;
}
bool isKeyK() {
	return keyK;
}
bool isKeyJ() {
	return keyJ;
}
bool isKeyL() {
	return keyL;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

double xposOld, yposOld;
double xpos, ypos;

void computeMatricesFromInputs(){
	
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	xposOld = xpos;
	yposOld = ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (xpos > xposOld) moveRight = true;
	else moveRight = false;
	if (ypos > yposOld) moveUp = true;
	else moveUp = false;
	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );
	
	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
		keyUp = true;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
		keyUp = false;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
		keyRight = true;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
		keyRight = false;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		modelMove[3][0] -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		modelMove[3][0] += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		modelMove[3][1] -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		modelMove[3][1] += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		modelMove[3][2] -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		modelMove[3][2] += 0.1f;
	}
	float tmpVal = 1;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		modelMove = rotate(modelMove, tmpVal, vec3(1, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		modelMove = rotate(modelMove, -tmpVal, vec3(1, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		modelMove = rotate(modelMove, tmpVal, vec3(0, 1, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		modelMove = rotate(modelMove, -tmpVal, vec3(0, 1, 0));
	}	
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		modelMove = rotate(modelMove, tmpVal, vec3(0, 0, 1));
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		modelMove = rotate(modelMove, -tmpVal, vec3(0, 0, 1));
	}
	//change color
	static bool isHold = false;
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS && !isHold) {
		colorChange = true;
		isHold = true;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
		isHold = false;
	}
	else {
		colorChange = false;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		leftClick = true;
	}
	else {
		leftClick = false;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		keyI = true;
	}
	else keyI = false;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		keyK = true;
	}
	else keyK = false;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		keyL = true;
	}
	else keyL = false;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		keyJ = true;
	}
	else keyJ = false;
	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	ray[0] = position;
	ray[1] = direction;
	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}