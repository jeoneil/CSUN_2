//#pragma once
# define __Windows__
# define GLM_ENABLE_EXPERIMENTAL

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Camera
{
public:

	// init stuff
	glm::mat4 I_matrix = glm::mat4(1.0f);		// identity matrix
	const glm::vec3 Y_AXIS = glm::vec3(0, 1, 0);
	
	float boundingRadius;

	// Matrices
	GLuint MVP;
	glm::mat4 M_matrix;		// model matrix
	glm::mat4 V_matrix;		// view matrix
	glm::mat4 P_matrix;		// projection matrix
	glm::mat4 MVP_matrix;	// model-view-projection matrix

	// lookAt info
	glm::vec3 eye, at, up;

	Camera(glm::vec3 eye) {
		printf("Initiating camera.\n");
		this->eye = eye;
		initMatrices();
		initCamera();
		
		printf("Camera initialized successfully.\n");
	}

	void initCamera() {
		at = glm::vec3(0.0f, 0.0f, 0.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		updateV_matrix();
	}

	void initMatrices() {
		M_matrix = I_matrix;
		V_matrix = I_matrix;
		P_matrix = I_matrix;
		MVP_matrix = I_matrix;
	}

	void updateMVP_matrix() {
		// update MVP matrix
		MVP_matrix = P_matrix * V_matrix * M_matrix;
	}
	void updateM_matrix(glm::mat4 newM_matrix) {
		M_matrix = newM_matrix;
	}

	void updateV_matrix() {
		V_matrix = glm::lookAt(eye, at, up);
	}

	void lookAtPlanet(glm::mat4 planetMatrix) {
		at = glm::vec3(planetMatrix[3][0], planetMatrix[3][1], planetMatrix[3][2]);
	}

	void updateP_matrix(GLfloat aspectRatio) {
		// update projection matrix
		P_matrix = glm::perspective(glm::radians(45.0f), aspectRatio, 1.0f, 100000.0f);
	}

	void move(glm::mat4 warshipM, glm::mat4 warshipR) {
		eye = getPosition(warshipM) - getIn(warshipR) * 1000.0f + getUp(warshipR) * 200.0f;
		at = getPosition(warshipM * glm::translate(I_matrix, glm::vec3(0.0f, 300.0f, 0.0f)));
		updateV_matrix();
	}
	void moveBackward() {
		
	}
	void moveRight() {
		
	}
	void moveLeft() {
		
	}
};

