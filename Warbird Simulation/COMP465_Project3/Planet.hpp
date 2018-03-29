# define __Windows__
# define GLM_ENABLE_EXPERIMENTAL

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Planet
{
public:
	
	// init stuff
	glm::mat4 I_matrix = glm::mat4(1.0f);		// identity matrix
	const glm::vec3 X_AXIS = glm::vec3(1, 0, 0),
					Y_AXIS = glm::vec3(0, 1, 0),
					Z_AXIS = glm::vec3(0, 0, 1);

	// rotation info
	GLfloat rotateAmount;	//0.1f
	GLfloat currRotation = 0;
	boolean orbital;		//false
	boolean rotates;
	boolean moon;

	// scale info
	GLfloat scaleAmount;

	// translate info
	glm::vec3 locationVertex;
	GLint orbitalSpeed;
	
	// Matrices
	glm::mat4 M_matrix;		// model matrix
	glm::mat4 R_matrix;		// rotation matrix
	glm::mat4 T_matrix;		// translation matrix
	glm::mat4 S_matrix;		// scale matrix
	glm::mat4 O_matrix;		// orientation matrix
	glm::mat4 skyboxM[6];	// model matrices for the skybox

	// duo's location
	glm::mat4 duoMatrix;
	glm::vec3 duoLocation;
	glm::vec3 duoAxis;

	// missile info
	int nMissiles;
	boolean dead = false;
	

	// for skybox
	Planet() {
		setupSkybox();
	}

	// for planets & missile bases
	Planet(GLfloat rotateAmount, GLfloat scaleAmount, glm::vec3 locationVertex, GLint orbitalSpeed, boolean orbital, boolean rotates, boolean moon) {
		this->rotateAmount = rotateAmount;
		this->scaleAmount = scaleAmount;
		this->locationVertex = locationVertex;
		this->orbitalSpeed = orbitalSpeed;
		this->orbital = orbital;
		this->rotates = rotates;
		this->moon = moon;
		this->nMissiles = 5;
		initMatrices();
	}

	void planetDisplayUpdates() {
		if (rotates) {
			updateR_matrix();
		}
		updateM_matrix();
	}

	void updateR_matrix() {
		currRotation += rotateAmount;
		if (currRotation >  2 * PI || currRotation < -(2 * PI)) 
			currRotation = 0.0f;
		if (moon) {
			R_matrix = glm::rotate(I_matrix, currRotation, glm::vec3(Y_AXIS));
		}
		else {
			R_matrix = glm::rotate(I_matrix, currRotation, glm::vec3(Y_AXIS));
		}
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

	glm::mat4 getOrientMatrix() {
		if (moon) {
			return glm::translate(I_matrix, duoAxis)
				* R_matrix * T_matrix;
		}
		else {
			updateM_matrix();
			return glm::translate(I_matrix, glm::vec3(M_matrix[3][0], M_matrix[3][1], M_matrix[3][2]))
				* R_matrix * T_matrix;
		}
	}

	void initMatrices() {
		M_matrix = I_matrix;
		R_matrix = I_matrix;
		T_matrix = glm::translate(I_matrix, locationVertex);
		S_matrix = glm::scale(I_matrix, glm::vec3(scaleAmount));
		duoAxis = glm::vec3(0, 9000, 0);
	}

	void setupSkybox() {
		// rotate skybox sides
	}
};

