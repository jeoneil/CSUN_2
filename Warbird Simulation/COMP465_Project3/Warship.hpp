# define __Windows__
# define GLM_ENABLE_EXPERIMENTAL

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# include "Model.hpp"
# include "Camera.hpp"
# include "Planet.hpp"

# define __INCLUDES465__
# endif

class Warship
{
public:

	// init stuff
	glm::mat4 I_matrix = glm::mat4(1.0f);		// identity matrix
	const glm::vec3 
		X_AXIS = glm::vec3(1, 0, 0),
		Y_AXIS = glm::vec3(0, 1, 0), 
		Z_AXIS = glm::vec3(0, 0, 1);

	// rotation info
	const GLfloat R_UNIT = 0.1f;
	GLfloat rotateAmount;	//0.1f
	GLfloat currRotationXZ = 0.0f;		// basic rotations
	GLfloat currRotationXY = 0.0f;		// rolling motions
	GLfloat currRotationYZ = 0.0f;		// pitching motions
	boolean orbital;		//false
	boolean rotates;
	boolean moon;
	boolean firstRotation = true;

	// scale info
	GLfloat scaleAmount;

	// translate info
	glm::vec3 locationVertex;
	GLint orbitalSpeed;
	GLfloat shipSpeed;
	boolean gravity = false;

	// Matrices
	glm::mat4 M_matrix;		// model matrix
	glm::mat4 R_matrix;		// rotation matrix
	glm::mat4 T_matrix;		// translation matrix
	glm::mat4 S_matrix;		// scale matrix
	glm::mat4 O_matrix;		// orientation matrix

							// duo's location
	glm::mat4 duoMatrix;
	glm::vec3 duoLocation;
	glm::vec3 duoAxis;

	// missile info
	int nMissiles;
	boolean dead = false;

	Warship (GLfloat rotateAmount, GLfloat scaleAmount, glm::vec3 locationVertex, GLint orbitalSpeed, boolean orbital, boolean rotates, boolean moon) {
		printf("Initiating planet.");
		this->rotateAmount = rotateAmount;
		this->scaleAmount = scaleAmount;
		this->locationVertex = locationVertex;
		this->orbitalSpeed = orbitalSpeed;
		this->orbital = orbital;
		this->rotates = rotates;
		this->moon = moon;
		this->shipSpeed = 10.0f;
		this->nMissiles = 9;
		initMatrices();
		printf("Project initiated successfully.");
	}

	void planetDisplayUpdates() {
		if (rotates) {
			updateR_matrix();
		}
		updateM_matrix();
	}

	void updateR_matrix() {
		R_matrix = glm::rotate(R_matrix, currRotationYZ, glm::vec3(X_AXIS));
		R_matrix = glm::rotate(R_matrix, currRotationXZ, glm::vec3(Y_AXIS));
		R_matrix = glm::rotate(R_matrix, currRotationXY, glm::vec3(Z_AXIS));
		
	}

	void updateM_matrix() {
		if (moon) {
			duoAxis = glm::vec3(duoMatrix[3][0], duoMatrix[3][1], duoMatrix[3][2]);
			M_matrix = glm::translate(I_matrix, duoAxis)
				* R_matrix * T_matrix * S_matrix;
		}
		else if (orbital) {
			M_matrix = R_matrix * T_matrix * S_matrix;
		}
		else {
			M_matrix = T_matrix * R_matrix * S_matrix;
		}
	}

	void updateT_matrix() {
		T_matrix = glm::translate(I_matrix, locationVertex);
	}

	void initMatrices() {
		R_matrix = I_matrix;
		updateT_matrix();
		S_matrix = glm::scale(I_matrix, glm::vec3(scaleAmount));
		duoAxis = glm::vec3(0, 9000, 0);
		updateM_matrix();
	}

	void warpCam(glm::mat4 cameraM) {
		locationVertex = getPosition(cameraM);
		updateT_matrix();
	}
	void moveForward() {
		locationVertex = getPosition(M_matrix) + getIn(R_matrix) * shipSpeed;
		updateT_matrix();
		updateM_matrix();
	}
	void moveBackward() {
		locationVertex = getPosition(M_matrix) + getIn(R_matrix) * -shipSpeed;
		updateT_matrix();
		updateM_matrix();
	}
	void moveRight() {
		currRotationXZ = -R_UNIT;
		updateR_matrix();
		updateM_matrix();
	}
	void moveLeft() {
		currRotationXZ = R_UNIT;
		updateR_matrix();
		updateM_matrix();
	}
	void pitchUp() {
		currRotationYZ = R_UNIT;
		updateR_matrix();
		updateM_matrix();
	}
	void pitchDown() {
		currRotationYZ = -R_UNIT;
		updateR_matrix();
		updateM_matrix();
	}
	void rollRight() {
		currRotationXY = -R_UNIT;
		updateR_matrix();
		updateM_matrix();
	}
	void rollLeft() {
		currRotationXY = R_UNIT;
		updateR_matrix();
		updateM_matrix();
	}

	void gravityEffect(GLfloat gravityForce, glm::vec3 planetLoc, glm::mat4 planetM) {
		glm::vec3 gravityVector = glm::normalize(getPosition(planetM) - getPosition(M_matrix));
		locationVertex = getPosition(M_matrix) + gravityVector * gravityForce;
		updateT_matrix();
		updateM_matrix();
	}
};

