# define __Windows__
# define GLM_ENABLE_EXPERIMENTAL

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# include "Model.hpp"
# include "Camera.hpp"
# include "Planet.hpp"
# include "Keyboard.hpp"

# define __INCLUDES465__
# endif

class Missile
{
public:

	// init stuff
	glm::mat4 I_matrix = glm::mat4(1.0f);		// identity matrix
	const glm::vec3
		X_AXIS = glm::vec3(1, 0, 0),
		Y_AXIS = glm::vec3(0, 1, 0),
		Z_AXIS = glm::vec3(0, 0, 1);

	const int UNUM_BASE = 1, SEC_BASE = 2, WARSHIP = 3;
	int number;

	// missile info
	boolean active = false;
	boolean smart = false;
	boolean dead = false;
	int age = 0;

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
	int target = -1;
	float missileSpeed = 10.0f;

	Missile (GLfloat rotateAmount, GLfloat scaleAmount, glm::vec3 locationVertex, GLint orbitalSpeed, boolean orbital, boolean rotates, boolean moon, int number) {
		this->rotateAmount = rotateAmount;
		this->scaleAmount = scaleAmount;
		this->locationVertex = locationVertex;
		this->orbitalSpeed = orbitalSpeed;
		this->orbital = orbital;
		this->rotates = rotates;
		this->moon = moon;
		this->shipSpeed = 10.0f;
		this->nMissiles = 9;
		this->number = number;
		initMatrices();
	}

	void planetDisplayUpdates() {
		updateM_matrix();
	}
	void planetDisplayUpdates(glm::mat4 baseM, glm::mat4 baseR) {
		R_matrix = baseR;
		T_matrix = glm::translate(I_matrix, getPosition(baseM));
		updateM_matrix();
	}

	void planetDisplayUpdates(glm::mat4 orientMatrix) {
		M_matrix = orientMatrix * S_matrix;
	}

	void updateR_matrix() {
		R_matrix = glm::rotate(R_matrix, currRotationYZ, glm::vec3(X_AXIS));
		R_matrix = glm::rotate(R_matrix, currRotationXZ, glm::vec3(Y_AXIS));
		R_matrix = glm::rotate(R_matrix, currRotationXY, glm::vec3(Z_AXIS));

	}

	void updateM_matrix() {
		M_matrix = T_matrix * R_matrix * S_matrix;
	}

	void updateT_matrix() {
		T_matrix = glm::translate(I_matrix, locationVertex);
	}

	void initMatrices() {
		M_matrix = I_matrix;
		R_matrix = I_matrix;
		updateT_matrix();
		S_matrix = glm::scale(I_matrix, glm::vec3(scaleAmount));
		duoAxis = glm::vec3(0, 9000, 0);
	}

	void track(glm::mat4 sourceR, glm::mat4 ubaseM, glm::mat4 sbaseM, glm::mat4 warM) {
		locationVertex = getPosition(M_matrix) + getIn(M_matrix) * missileSpeed;
		updateT_matrix();
		updateM_matrix();
		
		if (smart) {
			glm::vec3 objectAt, targetLoc, normTargetLoc, newRotationAxis;
			float newRotation, rotationRadian, newRadian;
			R_matrix = sourceR;
			if (target == -1) {
				if (number < 9) {
					// source is warship, find target
					target = SEC_BASE;
					if (distance(getPosition(M_matrix), getPosition(ubaseM)) <
						(distance(getPosition(M_matrix), getPosition(sbaseM)))) {
						target = UNUM_BASE;
					}
				}
				else {
					target = WARSHIP;
				}
			}
			if (target == UNUM_BASE)
				targetLoc = getPosition(ubaseM) - getPosition(M_matrix);
			if (target == SEC_BASE)
				targetLoc = getPosition(sbaseM) - getPosition(M_matrix);
			else if (target == WARSHIP)
				targetLoc = getPosition(warM) - getPosition(M_matrix);
			objectAt = getIn(R_matrix);
			normTargetLoc = glm::normalize(targetLoc);
			newRotationAxis = glm::normalize(glm::cross(normTargetLoc, objectAt));
			newRotation = newRotationAxis[0] + newRotationAxis[1] + newRotationAxis[2];
			rotationRadian = glm::dot(normTargetLoc, objectAt);
			newRadian = (2 * PI) - glm::acos(rotationRadian);
			if (colinear(objectAt, normTargetLoc, .08) &&
				distance(getPosition(M_matrix) + objectAt, targetLoc) > distance(getPosition(M_matrix), targetLoc)) {
				R_matrix = glm::rotate(R_matrix, PI, getUp(R_matrix));
			}
			else {
				if (distance(normTargetLoc, objectAt) < .5) {}
				else {
					R_matrix = glm::rotate(R_matrix, newRadian, newRotationAxis);
				}
			}
		}
	}

	boolean collidedWith(glm::mat4 targetM, float targetBoundary) {
		if (distance(getPosition(targetM), getPosition(M_matrix)) < (targetBoundary + 30.0f)) {
			return true;
		}
		return false;
	}

	boolean detectWarship(glm::mat4 warM) {
		if (distance(getPosition(M_matrix), getPosition(warM)) < 3000) {
			return true;
		}
		return false;
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