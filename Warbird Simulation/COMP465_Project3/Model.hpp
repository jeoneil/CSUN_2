





//#pragma once
# define __Windows__
# define GLM_ENABLE_EXPERIMENTAL

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# include "../includes465/texture465.hpp"
# define __INCLUDES465__
# endif

//# include "../../includes465/texture465.hpp"

#define nPlanets 5
#define nBases 2
#define numMissiles 19
#define nModels 27

class Model {

public:
	// -------------- VARIABLES --------------

	// names
	const int
		RUBER = 0,
		UNUM = 1,
		DUO = 2,
		PRIMUS = 3,
		SECUNDUS = 4,
		UNUM_BASE = 5,
		DUO_BASE = 6,
		WARSHIP = 26,
		SKYBOX = 27;

	// Model info
	char * modelFile[nModels + 1] = {
		"ruber.tri",
		"unum.tri",
		"duo.tri",
		"primus.tri",
		"secundus.tri",
		"cube-1-1-1.tri",
		"cube-1-1-1.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"missile.tri",
		"spaceShip-bs100.tri",
		"skybox.raw"
	};
	const GLuint nVertices[nModels + 1] = {
		264 * 3,
		264 * 3,
		264 * 3,
		264 * 3,
		264 * 3,
		12 * 3,
		12 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		13 * 3,
		996 * 3,
		2 * 3 };
	float boundingRadius[nModels];

	// 3 Shader stuff
	GLuint shaderProgram;
	char* vertexShaderFile = "customVertex.glsl";
	char* fragmentShaderFile = "customFragment.glsl";

	char* shaderVertex = "vPosition";
	char* shaderColor = "vColor";
	char* shaderNormal = "vNormal";
	char* shaderTexCoord = "vTextCoord";

	GLuint vPosition[nModels + 1], vColor[nModels + 1], vNormal[nModels + 1], vTexCoord;

	// 4 VAO's & VBO's
	GLuint vao[nModels + 1], vbo[nModels + 1];

	// 5 handles
	GLuint MVP_handle;
	GLuint MV_handle;
	GLuint normal_handle;
	GLint isTexture_handle, PLPos_handle, PLIntensity_handle, HLPos_handle, HLIntensity_handle, PLOn_handle, HLOn_handle, ALOn_handle, DebugOn_handle, texture_handle;
	char* MVP_handleName = "ModelViewProjection";
	char* MV_handleName = "ModelView";
	char* normal_handleName = "NormalMatrix";
	char* isTexture_handleName = "isTexture";

	Model() {
		printf("Initiating model.\n");
		initShaders();
		printf("Shaders initialized.\n");
		initVertexArrays();
		printf("Vertex Arrays initialized.\n");
		loadBuffers();
		printf("Buffers loaded.\n");
		//loadSkybox();
		//printf("Skybox loaded.\n");
		initHandles();
		printf("Handles initialized.\n");
		printf("Model initiation successful.\n");
	}

	void loadBuffers() {
		for (int i = 0; i < nModels; i++) {
			boundingRadius[i] = loadModelBuffer(modelFile[i], nVertices[i], vao[i], vbo[i], shaderProgram,
				vPosition[i], vColor[i], vNormal[i],
				shaderVertex, shaderColor, shaderNormal);
		}
	}

	// 6 texture info
	GLuint ibo;
	const GLfloat skyboxPoints[16] = {
		-1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,
	};

	const unsigned int indices[6] = {
		0, 1, 2,
		1, 2, 3,
	};

	const GLfloat texCoords[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};



	void initShaders() {
		// Load the Shader
		shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
		glUseProgram(shaderProgram);
	}

	void initVertexArrays() {
		// Initialize VAOs & VBOs
		glGenVertexArrays(nModels, vao);
		glGenBuffers(nModels, vbo);
	}

	void loadSkybox() {
		GLuint textures = -1;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(vao[SKYBOX]);

		glEnableVertexAttribArray(vbo[SKYBOX]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[SKYBOX]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxPoints) + sizeof(texCoords), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(skyboxPoints), skyboxPoints);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(skyboxPoints), sizeof(texCoords), texCoords);

		vPosition[SKYBOX] = glGetAttribLocation(shaderProgram, shaderVertex);
		glVertexAttribPointer(vPosition[SKYBOX], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(vPosition[SKYBOX]);

		vTexCoord = glGetAttribLocation(shaderProgram, shaderTexCoord);
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(skyboxPoints)));
		glEnableVertexAttribArray(vTexCoord);

		GLuint texture = loadRawTexture(textures, modelFile[SKYBOX], 1080, 1080);
		if (texture = 0) {
			printf("Error: could not load text file.\n");
		} // get uniform location for texture
		else printf("Textures loaded successfully.\n");
	}

	void initHandles() {
		// Set MVP
		MVP_handle = glGetUniformLocation(shaderProgram, MVP_handleName);
		MV_handle = glGetUniformLocation(shaderProgram, MV_handleName);
		normal_handle = glGetUniformLocation(shaderProgram, "NormalMatrix");
		isTexture_handle = glGetUniformLocation(shaderProgram, "isTexture");
		PLPos_handle = glGetUniformLocation(shaderProgram, "PointLightPosition");
		PLIntensity_handle = glGetUniformLocation(shaderProgram, "PointLightIntensity");
		HLPos_handle = glGetUniformLocation(shaderProgram, "HeadLightPosition");
		HLIntensity_handle = glGetUniformLocation(shaderProgram, "HeadLightIntensity");
		PLOn_handle = glGetUniformLocation(shaderProgram, "PointLightOn");
		HLOn_handle = glGetUniformLocation(shaderProgram, "HeadLightOn");
		ALOn_handle = glGetUniformLocation(shaderProgram, "AmbientOn");
		DebugOn_handle = glGetUniformLocation(shaderProgram, "DebugOn");
		texture_handle = glGetUniformLocation(shaderProgram, "Texture");
	}

	void submitLightData(boolean ALOn, boolean HLOn, boolean PLOn, glm::mat4 cameraM, glm::mat4 cameraV) {
		glm::mat3 cameraMV = cameraV * cameraM;
		glm::vec3 ruberLocation = glm::vec3(0.1f, 1000.0f, 0.1f);
		glm::vec3 PLIntensity = glm::vec3(1.0f, 0.5f, 0.0f);	// orange light
		glm::vec3 HLIntensity = glm::vec3(0.2f, 0.2f, 0.7f);	// blue light
		glUniformMatrix4fv(MV_handle, 1, GL_FALSE, glm::value_ptr(cameraMV));
		glUniformMatrix3fv(normal_handle, 1, GL_FALSE, glm::value_ptr(glm::mat3(cameraMV)));
		glUniform3fv(PLPos_handle, 1, glm::value_ptr(ruberLocation));
		glUniform3fv(PLIntensity_handle, 1, glm::value_ptr(PLIntensity));
		glUniform3fv(HLPos_handle, 1, glm::value_ptr(getPosition(cameraV)));
		glUniform3fv(HLIntensity_handle, 1, glm::value_ptr(HLIntensity));
		glUniform1f(ALOn_handle, ALOn);
		glUniform1f(HLOn_handle, HLOn);
		glUniform1f(PLOn_handle, PLOn);
	}

	void submitTextureInfo(glm::mat4 cameraM, glm::mat4 cameraV) {
		glm::mat3 cameraMV = cameraV * cameraM;
		glUniformMatrix4fv(MV_handle, 1, GL_FALSE, glm::value_ptr(cameraMV));
		glUniformMatrix3fv(normal_handle, 1, GL_FALSE, glm::value_ptr(glm::mat3(cameraMV)));
		glBindVertexArray(vao[SKYBOX]);
		glUniform1f(isTexture_handle, true);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, nVertices[SKYBOX], GL_UNSIGNED_INT, BUFFER_OFFSET(0));		// causes crash
	}

	void submitNewMVP_matrix(glm::mat4 newMVP_matrix) {
		// Send new matrix info
		glUniformMatrix4fv(MVP_handle, 1, GL_FALSE, glm::value_ptr(newMVP_matrix));
	}
};