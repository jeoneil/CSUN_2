/*
Jack O'Neil
10 December, 2017
COMP 465
Lab Project
*/

# define __Windows__
# define GLM_ENABLE_EXPERIMENTAL
# include "../includes465/include465.hpp"
# include "../includes465/texture465.hpp"
# include "Model.hpp"
# include "Camera.hpp"
# include "Planet.hpp"
# include "Missile.hpp"
# include "Warship.hpp"

#define nPlanets 5
#define nBases 2
#define nModels 27
#define nCameras 5
#define numMissiles 19

// -------------- VARIABLES --------------

// Window
GLfloat aspectRatio;
int timerDelay = 40; // 40 millisecond delay approximates 35 fps
int frameCount = 0, fps = 0, updateCount = 0, ups = 0;
double timeInterval, currentTime, lastTime;


// MODEL INFO

// init stuff
glm::mat4 I_matrix = glm::mat4(1.0f);		// identity matrix
const glm::vec3 Y_AXIS = glm::vec3(0, 1, 0);
boolean firstMovement = true;

// keyboard input info
boolean warpUnumCam = true;
boolean gravityOn = false, warshipFiring = false, unumFiring = false, secFiring = false,
WreadyToFire = true, UreadyToFire = true, SreadyToFire = true;
boolean gameOver = false;
boolean win = false;

const int
ACE = 5,
PILOT = 40,
TRAINEE = 100,
DEBUG = 500;
int TQ = PILOT;


// light info
boolean pointLightOn = true, headLightOn = true, ambientLightOn = true, debugLightOn = false;

// movement info
boolean
forward = false,
backward = false,
right = false,
left = false,
up = false,
down = false,
rollRight = false,
rollLeft = false;

// names
const int
RUBER = 0,
UNUM = 1,
DUO = 2,
PRIMUS = 3,
SECUNDUS = 4,
UNUM_BASE = 5,
SECUNDUS_BASE = 6,
FIRST_WAR_MISSILE = 7,
FIRST_UNUM_MISSILE = 16,
FIRST_SEC_MISSILE = 21,
WARSHIP = 26,
SKYBOX = 27;

// locations
glm::vec3 location[nModels] = {
	glm::vec3(0, 0, 0),
	glm::vec3(4000, 0, 0),
	glm::vec3(9000, 0, 0),
	glm::vec3(11000 - 9000, 0, 0),
	glm::vec3(12000 - 9000, 0, 0),
	glm::vec3(4000, 300, 0),
	glm::vec3(12000 - 9000, 200, 0),

	glm::vec3(15000, 0, 0),
	glm::vec3(15000, 0, 0),
	glm::vec3(15000, 0, 0),
	glm::vec3(15000, 0, 0),
	glm::vec3(15000, 0, 0),
	glm::vec3(15000, 0, 0),
	glm::vec3(15000, 0, 0),
	glm::vec3(15000, 0, 0),
	glm::vec3(15000, 0, 0),
	glm::vec3(4000, 300, 0),
	glm::vec3(4000, 300, 0),
	glm::vec3(4000, 300, 0),
	glm::vec3(4000, 300, 0),
	glm::vec3(4000, 300, 0),
	glm::vec3(12000 - 9000, 200, 0),
	glm::vec3(12000 - 9000, 200, 0),
	glm::vec3(12000 - 9000, 200, 0),
	glm::vec3(12000 - 9000, 200, 0),
	glm::vec3(12000 - 9000, 200, 0),

	glm::vec3(15000, 0, 0)
};

// radii
GLfloat missileScale = 50.0f;
GLfloat scale[nModels] = { 2000, 200, 400, 100, 150, 50, 35,
missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale, missileScale,
100 };

// rotation speeds (radians per update)
GLfloat rotationSpeed[nModels] = {
	0.0f, 0.004f, 0.002f, 0.010f, 0.030f, 0.004f, 0.030f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f };

// axii of rotation
boolean moon[nModels] = { false, false, false, true, true, false, true,
false, false, false, false, false, false, false, false, false, false,
false, false, false, false, false, false, false, false, false,
false };

// movement speed (per update)
GLint orbitSpeed[nModels] = { 0, 8, 16, 3, 1 , 8, 1,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0 };

// orbital?
boolean orbital[nModels] = { false, true, true, true, true, true, true,
false, false, false, false, false, false, false, false, false, false,
false, false, false, false, false, false, false, false, false,
false };

// rotates?
boolean rotates[nModels] = { false, true, true, true, true, true, true,
false, false, false, false, false, false, false, false, false, false,
false, false, false, false, false, false, false, false, false,
false };


// missile info
int nextWarMissile = FIRST_WAR_MISSILE, nextUnumMissile = FIRST_UNUM_MISSILE, nextSecMissile = FIRST_SEC_MISSILE;


// Camera names
const int
FRONT = 0, TOP = 1, UNUM_CAM = 2, DUO_CAM = 3, WARSHIP_CAM = 4;

// camera locations
glm::vec3 cameraLocation[nCameras] = {
	glm::vec3(0, 10000, 20000),
	glm::vec3(0, 20000, 0.1f),
	glm::vec3(-4000, 0, -4000),
	glm::vec3(-4000, 0, -4000),
	glm::vec3(15000 + 1000, 200, 0) };

// CAMERA INFO
int currCamera = WARSHIP_CAM;

// classes
Camera* camera[nCameras];
Model* model;
Planet* planet[nPlanets + nBases + 1];
Missile* missile[numMissiles];
Warship* warship;

// -------------- FUNCTIONS --------------

void updateTitle() {
	char titleStr[100];

	if (gameOver) {
		sprintf(titleStr, "GAME OVER.");
	}
	else if (win) {
		sprintf(titleStr, "YOU HAVE WON!");
	}

	else {
		char camStr[10];
		switch (currCamera) {
		case TOP: sprintf(camStr, "Top"); break;
		case FRONT: sprintf(camStr, "Front"); break;
		case UNUM_CAM: sprintf(camStr, "Unum"); break;
		case DUO_CAM: sprintf(camStr, "Duo"); break;
		case WARSHIP_CAM: sprintf(camStr, "Warship"); break;
		}
		sprintf(titleStr, "Warbird %d     Unum %d     Secundus %d     U/S %d     F/S %d     View %s",
			warship->nMissiles, planet[UNUM_BASE]->nMissiles, planet[SECUNDUS_BASE]->nMissiles, ups, fps, camStr);
	}


	glutSetWindowTitle(titleStr);
}

void checkTimeInterval() {
	frameCount++;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeInterval = currentTime - lastTime;
	if (timeInterval >= 1000) {
		fps = (int)(frameCount / (timeInterval / 1000.0f));
		ups = (int)(updateCount) / (timeInterval / 1000.0f);
		lastTime = currentTime;
		frameCount = 0;
		updateCount = 0;
	}

}

void movement() {
	if (forward) warship->moveForward();
	else if (backward) warship->moveBackward();
	if (right) warship->moveRight();
	else if (left) warship->moveLeft();
	if (up) warship->pitchUp();
	else if (down) warship->pitchDown();
	if (rollRight) warship->rollRight();
	else if (rollLeft) warship->rollLeft();
	camera[WARSHIP_CAM]->move(warship->M_matrix, warship->R_matrix);
	forward = backward = right = left = up = down = rollRight = rollLeft = false;
}

void display(void) {
	// Clear scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);

	// update MVP and submit
	for (int i = 0; i < nModels; i++) {
		for (int j = 0; j < nCameras; j++) {
			if (i == WARSHIP) {
				camera[j]->updateM_matrix(warship->M_matrix);
			}
			else if (i < nPlanets + nBases) {
				camera[j]->updateM_matrix(planet[i]->M_matrix);
			}
			else {
				camera[j]->updateM_matrix(missile[i - (nPlanets + nBases)]->M_matrix);
			}
			camera[j]->updateV_matrix();
			camera[j]->updateMVP_matrix();
		}
		model->submitNewMVP_matrix(camera[currCamera]->MVP_matrix);

		// Draw
		glBindVertexArray(model->vao[i]);
		glDrawArrays(GL_TRIANGLES, 0, model->nVertices[i]);
	}

	// submit skybox MVP
	//for (int i = 0; i < 6; i++) {
	//	for (int j = 0; j < nCameras; j++) {
	//		camera[j]->updateM_matrix(planet[SKYBOX]->skyboxM[i]);
	//		camera[j]->updateV_matrix();
	//		camera[j]->updateMVP_matrix();
	//	}
	//	model->submitNewMVP_matrix(camera[currCamera]->MVP_matrix);
	//	model->submitTextureInfo(camera[currCamera]->M_matrix, camera[currCamera]->V_matrix);
	//}

	// submit light data
	model->submitLightData(ambientLightOn, headLightOn, pointLightOn, 
		camera[currCamera]->M_matrix, camera[currCamera]->V_matrix);

	checkTimeInterval();
	updateTitle();
	glutSwapBuffers();
}

void init(void) {
	printf("Entering init().\n");
	model = new Model();

	// models
	for (int i = 0; i < nPlanets + nBases; i++) {
		planet[i] = new Planet(rotationSpeed[i],
			scale[i] / model->boundingRadius[i],
			location[i], orbitSpeed[i], orbital[i], rotates[i], moon[i]);
	}
	for (int i = 0; i < numMissiles; i++) {
		missile[i] = new Missile(rotationSpeed[i + nPlanets + nBases],
			missileScale / model->boundingRadius[i + nPlanets + nBases],
			location[i + nPlanets + nBases], orbitSpeed[i + nPlanets + nBases],
			orbital[i + nPlanets + nBases], rotates[i + nPlanets + nBases],
			moon[i + nPlanets + nBases], i);
	}
	warship = new Warship(rotationSpeed[WARSHIP],
		scale[WARSHIP] / model->boundingRadius[WARSHIP],
		location[WARSHIP], orbitSpeed[WARSHIP], orbital[WARSHIP],
		rotates[WARSHIP], moon[WARSHIP]);
	warship->R_matrix = glm::rotate(I_matrix, PI / 2, Y_AXIS);

	// skybox
	planet[nPlanets + nBases] = new Planet();

	// cameras
	for (int i = 0; i < nCameras; i++) {
		camera[i] = new Camera(cameraLocation[i]);
		if (i == UNUM_CAM) {
			camera[i]->lookAtPlanet(planet[UNUM]->M_matrix);
		}
		if (i == DUO_CAM) {
			camera[i]->lookAtPlanet(planet[DUO]->M_matrix);
		}
		if (i == WARSHIP_CAM) {
			camera[i]->lookAtPlanet(warship->M_matrix);
		}

	}

	// glEnable
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	printf("Exiting init().\n");
}

// set viewport and projectionMatrix on window resize events
void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	aspectRatio = (GLfloat)width / (GLfloat)height;

	// set projection matrix
	for (int i = 0; i < nCameras; i++) {
		camera[i]->updateP_matrix(aspectRatio);
	}
}

// Use with Idle and intervalTimer functions 
void update(void) {
	// model matrix updates
	glm::mat4 duoMatrix = planet[DUO]->M_matrix;
	glm::vec3 duoLocation = planet[DUO]->locationVertex;
	for (int i = 0; i < nPlanets + nBases; i++) {
		if (i == PRIMUS || i == SECUNDUS || i == SECUNDUS_BASE) {
			planet[i]->duoMatrix = duoMatrix;
			planet[i]->duoLocation = duoLocation;
		}
		if (i == UNUM_BASE || i == SECUNDUS_BASE) {
			if (planet[i]->dead) {
				planet[i]->T_matrix = glm::translate(I_matrix, location[RUBER]);
			}
		}
		planet[i]->planetDisplayUpdates();
		if (i == DUO) {
			duoMatrix = planet[i]->M_matrix;
			duoLocation = planet[i]->locationVertex;
		}
	}

	// warship missile firing handling
	if (warshipFiring && nextWarMissile < FIRST_UNUM_MISSILE && missile[WreadyToFire - (nPlanets + nBases)] && WreadyToFire) {
		missile[nextWarMissile - (nPlanets + nBases)]->active = true;
		printf("Warship missile %d is now active\n", nextWarMissile);
		nextWarMissile++;
		warshipFiring = false;
		WreadyToFire = false;
		warship->nMissiles--;
		if (warship->nMissiles == 0) gameOver = true;	// check here if warship has ran out of missiles
	}
	// base missile firing handling
	else if (unumFiring && nextUnumMissile < FIRST_SEC_MISSILE && missile[nextUnumMissile - (nPlanets + nBases)] && UreadyToFire) {
		missile[nextUnumMissile - (nPlanets + nBases)]->active = true;
		printf("Unum base missile %d is now active\n", nextUnumMissile);
		nextUnumMissile++;
		unumFiring = false;
		UreadyToFire = false;
		planet[UNUM_BASE]->nMissiles--;
	}
	else if (secFiring && nextSecMissile < WARSHIP && missile[nextSecMissile - (nPlanets + nBases)] && SreadyToFire) {
		missile[nextSecMissile - (nPlanets + nBases)]->active = true;
		printf("Secundus base missile %d is now active\n", nextSecMissile);
		nextSecMissile++;
		secFiring = false;
		SreadyToFire = false;
		planet[SECUNDUS_BASE]->nMissiles--;
	}

	for (int i = 0; i < numMissiles; i++) {
		if (!missile[i]->active) {	// handle dormant missiles
			if (i < 9) {	// warship missiles
				missile[i]->planetDisplayUpdates(warship->M_matrix, warship->R_matrix);
			}
			else if (i > 9 && i < 14) {		// unum base missiles
				if (missile[i]->detectWarship(warship->M_matrix)) {
					unumFiring = true;
				}
				//missile[i]->planetDisplayUpdates(planet[UNUM_BASE]->getOrientMatrix());
				missile[i]->planetDisplayUpdates(planet[UNUM_BASE]->M_matrix, planet[UNUM_BASE]->R_matrix);
			}
			else {	// secundus base missiles
				if (missile[i]->detectWarship(warship->M_matrix)) {
					secFiring = true;
				}
				missile[i]->planetDisplayUpdates(planet[SECUNDUS_BASE]->M_matrix, planet[SECUNDUS_BASE]->R_matrix);
			}
		}
		else if (missile[i]->active && !missile[i]->dead) {		// handle active missiles
			missile[i]->age++;
			if (missile[i]->age == 200) {	// missile is ready to go smart
				missile[i]->smart = true;
			}
			if (missile[i]->age == 2000) {	// missile has reached the end of its life
				missile[i]->dead = true;
				missile[i]->T_matrix = glm::translate(I_matrix, location[RUBER]);
				if (i < 9) {
					WreadyToFire = true;
				}
				if (i > 9 && i < 14) {
					UreadyToFire = true;
				}
				else SreadyToFire = true;
			}
			else {	// general active missile
				if (i < 9) {
					missile[i]->track(warship->R_matrix, planet[UNUM_BASE]->M_matrix,
						planet[SECUNDUS_BASE]->M_matrix, warship->M_matrix);
					if (missile[i]->collidedWith(planet[UNUM_BASE]->M_matrix, model->boundingRadius[UNUM_BASE])) {
						planet[UNUM_BASE]->dead = true;			// unum base destroyed
						missile[i]->dead = true;
						WreadyToFire = true;
						for (int j = 9; j < 14; j++) {	// can't fire anymore
							missile[j]->dead = true;
						}
					}
					if (missile[i]->collidedWith(planet[SECUNDUS_BASE]->M_matrix, model->boundingRadius[SECUNDUS_BASE])) {
						planet[SECUNDUS_BASE]->dead = true;		// secundus base destroyed
						missile[i]->dead = true;
						WreadyToFire = true;
						for (int j = 14; j < numMissiles; j++) {	// can't fire anymore
							missile[j]->dead = true;
						}
					}
				}
				else if (i > 9 && i < 14) {
					missile[i]->track(planet[UNUM_BASE]->R_matrix, planet[UNUM_BASE]->M_matrix,
						planet[SECUNDUS_BASE]->M_matrix, warship->M_matrix);
					if (missile[i]->collidedWith(warship->M_matrix, model->boundingRadius[WARSHIP])) {
						gameOver = true;						// warship destroyed
						missile[i]->dead = true;
						UreadyToFire = true;
						for (int j = 0; j < 9; j++) {	// can't fire anymore
							missile[j]->dead = true;
						}
					}
				}
				else {
					missile[i]->track(planet[SECUNDUS_BASE]->R_matrix, planet[UNUM_BASE]->M_matrix,
						planet[SECUNDUS_BASE]->M_matrix, warship->M_matrix);
					if (missile[i]->collidedWith(warship->M_matrix, model->boundingRadius[WARSHIP])) {
						gameOver = true;						// warship destroyed
						missile[i]->dead = true;
						SreadyToFire = true;
						for (int j = 0; j < 9; j++) {	// can't fire anymore
							missile[j]->dead = true;
						}
					}
				}
			}
			missile[i]->planetDisplayUpdates();
		}
		else if (missile[i]->dead) {
			// dead missile "disappears"
			missile[i]->T_matrix = glm::translate(I_matrix, location[RUBER]);
			missile[i]->planetDisplayUpdates();

		}

	}
	if (!firstMovement && gravityOn) {
		warship->gravityEffect(5.63f, planet[UNUM]->locationVertex, planet[UNUM]->M_matrix);
		warship->gravityEffect(1.11f, planet[DUO]->locationVertex, planet[DUO]->M_matrix);
	}
	else firstMovement = false;
	movement();
	warship->planetDisplayUpdates();

	camera[UNUM_CAM]->lookAtPlanet(planet[UNUM]->M_matrix);
	camera[DUO_CAM]->lookAtPlanet(planet[DUO]->M_matrix);
	camera[WARSHIP_CAM]->lookAtPlanet(warship->M_matrix);

	// check if game is won
	if (planet[UNUM_BASE]->dead && planet[SECUNDUS_BASE]->dead) {
		win = true;
	}

	updateCount++;
	glutPostRedisplay();
}

// Estimate FPS, use for fixed interval timer driven animation
void intervalTimer(int i) {
	glutTimerFunc(TQ, intervalTimer, 1); // reset timerDelay
	update();  // fixed interval timer
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033: case 'q':  case 'Q': exit(EXIT_SUCCESS); break;
	case 'w':
		if (warpUnumCam) {
			warship->warpCam(camera[UNUM_CAM]->V_matrix);
			camera[WARSHIP_CAM]->move(warship->M_matrix, warship->R_matrix);
			warpUnumCam = false;
		}
		else {
			warship->warpCam(camera[DUO_CAM]->V_matrix);
			camera[WARSHIP_CAM]->move(warship->M_matrix, warship->R_matrix);
			warpUnumCam = true;
		}
		break;
	case 'f':
		warshipFiring = true;
		break;
	case 'g':
		gravityOn = !gravityOn;
		break;
	case 'v':
		if (currCamera == nCameras - 1)
			currCamera = 0;
		else
			currCamera++;
		break;
	case 'x':
		if (currCamera == 0)
			currCamera = nCameras - 1;
		else
			currCamera--;
		break;
	case 't':
		if (TQ == ACE) TQ = PILOT;
		else if (TQ == PILOT) TQ = TRAINEE;
		else if (TQ == TRAINEE) TQ = DEBUG;
		else TQ = ACE;
		break;
	case 's':
		if (warship->shipSpeed == 10)
			warship->shipSpeed = 50;
		else if (warship->shipSpeed == 50)
			warship->shipSpeed = 200;
		else warship->shipSpeed = 10;
		break;
	case 'a':
		ambientLightOn = !ambientLightOn;
		break;
	case 'p':
		pointLightOn = !pointLightOn;
		break;
	case 'h':
		headLightOn = !headLightOn;
		break;
	}
}

void specialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
			up = true;
		}
		else {
			forward = true;
		}
	}
	else if (key == GLUT_KEY_DOWN) {
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
			down = true;
		}
		else {
			backward = true;
		}
	}
	if (key == GLUT_KEY_RIGHT) {
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
			rollRight = true;
		}
		else {
			right = true;
		}
	}
	else if (key == GLUT_KEY_LEFT) {
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
			rollLeft = true;
		}
		else {
			left = true;
		}
	}
}

int main(int argc, char* argv[]) {
	//freopen("out.txt", "w", stdout);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	// Uncomment the following line to force OpenGL & GLSL 3.3
	// glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Starter source file for 465L");
	// initialize and verify glew
	glewExperimental = GL_TRUE;  // needed my home system 
	GLenum err = glewInit();
	if (GLEW_OK != err)
		printf("GLEW Error: %s \n", glewGetErrorString(err));
	else {
		printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
		printf("OpenGL %s, GLSL %s\n",
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	// initialize scene here
	init();
	// set glut callback functions here
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutTimerFunc(TQ, intervalTimer, 1);  // keep the window up
	glutMainLoop();
	printf("Done!\n");
	return 0;
}