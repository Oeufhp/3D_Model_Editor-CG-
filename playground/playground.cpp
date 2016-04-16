// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
// Include GLEW
#include <GL/glew.h>
#include <iostream>
// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <objloader.hpp>
#include <common/vboindexer.hpp>

int main(void)
{
	char  filepath[128];
	//char ddspath[128];
	GLfloat a1, a2, a3;
	printf("FileName >> ");
	scanf("%s", &filepath);
	/*printf("ddsPath >> ");
	scanf("%s", &ddspath);*/
	printf("Start Position >> ");
	scanf("%f %f %f", &a1, &a2, &a3);
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "3D model editor by OeufEiei", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
	//GLuint programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Load the texture
	//GLuint Texture = loadDDS(ddspath);

	// Get a handle for our "myTextureSampler" uniform
	//GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	int res = loadOBJ(filepath, vertices, uvs, normals);
	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	for (int i = 0; i < vertices.size(); i++) {
		printf("%f %f %f\n", vertices[i].x, vertices[i].y, vertices[i].z);
	}
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	//------------------------------color custom--------------------------------------
	GLfloat g_color_buffer_data[10000];
	for (int i = 0; i < res * 3; i++) {
		g_color_buffer_data[3 * i] = 1.0;
		g_color_buffer_data[3 * i + 1] = 0.0;
		g_color_buffer_data[3 * i + 2] = 0.0;
	}
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	//-------------------------------end color custom-----------------------------------
	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	float R, G, B;
	do {
		if (isKeyI || isKeyJ || isKeyK || isKeyL) {
			//std::cout << (*getRay()).x <<" "<< (*getRay()).y << " " << (*getRay()).z << " with " << (*(getRay() + 1)).x << " " << (*(getRay() + 1)).y << " " << (*(getRay() + 1)).z << std::endl;
			vec3 checkpoint;
			vec3 camera, direction;
			camera = *(getRay());
			direction = *(getRay() + 1);
			float t;
			//std::cout << "Check point!" << std::endl;
			for (int i = 0; i < vertices.size(); i++) {
				t = (vertices[i].x - camera.x) / direction.x;
				if (std::abs(vertices[i].y - (camera.y + direction.y*t)) <= 0.1) {
					if (std::abs(vertices[i].z - (camera.z + direction.z*t)) <= 0.1) {
						std::cout << "Vectex modify is " << i << std::endl;
						vec3 tmpVec = vertices[i];
						if (isKeyI()) {
							for (int j = 0; j < vertices.size(); j++) {
								if (tmpVec == vertices[j]) {
									//vertices[j].x += 1.0f;
									vertices[j].y += 0.5f;
									//vertices[j].z += 1.0f;
								}
							}
						}
						if (isKeyK()) {
							for (int j = 0; j < vertices.size(); j++) {
								if (tmpVec == vertices[j]) {
									//vertices[j].x += 1.0f;
									vertices[j].y -= 0.5f;
									//vertices[j].z += 1.0f;
								}
							}
						}
						if (isKeyJ()) {
							for (int j = 0; j < vertices.size(); j++) {
								if (tmpVec == vertices[j]) {
									vertices[j].x -= 0.5f;
									//vertices[j].y -= 0.5f;
									//vertices[j].z += 1.0f;
								}
							}
						}
						if (isKeyL()) {
							for (int j = 0; j < vertices.size(); j++) {
								if (tmpVec == vertices[j]) {
									vertices[j].x += 0.5f;
									//vertices[j].y += 0.5f;
									//vertices[j].z += 1.0f;
								}
							}
						}

						break;
					}
				}
			}
			glGenBuffers(1, &vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		}
		if (getColorChange()) {
			for (int i = 0; i < res; i++) {
				R = static_cast<float> (rand()) / RAND_MAX;
				G = static_cast<float> (rand()) / RAND_MAX;
				B = static_cast<float> (rand()) / RAND_MAX;
				g_color_buffer_data[9 * i] = R;
				g_color_buffer_data[9 * i + 1] = G; //G
				g_color_buffer_data[9 * i + 2] = B; //B

				g_color_buffer_data[9 * i + 3] = R; 
				g_color_buffer_data[9 * i + 4] = G;
				g_color_buffer_data[9 * i + 5] = B;

				g_color_buffer_data[9 * i + 6] = R;
				g_color_buffer_data[9 * i + 7] = G;
				g_color_buffer_data[9 * i + 8] = B;
			}
			glGenBuffers(1, &colorbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
		}



		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrixOrigin = glm::translate(mat4(1.0f), vec3(a1, a2, a3));
		glm::mat4 ModelMatrix = getModelMove() * ModelMatrixOrigin;
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		//glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
		//--------------------custom color-------------------------
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);
		//---------------------------------------------------------
		/*
		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);
		*/
		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//	glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	//	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

