#include <ctime> 
#include <iostream> 
#include <algorithm>
#include <cmath>

#include "EShape.h"
#include "FrameBuffer.h"
#include "Raytracer.h"
#include "IScene.h"
#include "Light.h"
#include "Camera.h"
#include "Utilities.h"
#include "VertexOps.h"

PositionalLightPtr theLight = new PositionalLight(glm::vec3(2, 1, 3), pureWhiteLight);
std::vector<LightSourcePtr> lights = { theLight };

glm::vec3 position(0, 1, 4);
float angle = glm::radians(180.0f); // heading
float az = glm::radians(180.0f);
float el = 0.0f;
bool isMoving = false;
bool twoViewOn = false;
const float SPEED = 0.1;

FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

std::vector<glm::vec4> planeCorners() {
	std::vector<glm::vec4> points;
	points.push_back(glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));
	points.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	points.push_back(glm::vec4(1.0f, 3.0f, 1.0f, 1.0f));
	points.push_back(glm::vec4(-1.0f, 3.0f, 1.0f, 1.0f));
	return points;
}

EShapeData board = EShape::createECheckerBoard(copper, tin, 10, 10, 10);
EShapeData plane = EShape::createEPlanes(tin, planeCorners());
EShapeData cone = EShape::createECone(gold, 1.0f, 1.0f, 16);
EShapeData cylinderY = EShape::createECylinder(gold, 1.0f, 1.0f, 16);
EShapeData cylinderZ = EShape::createECylinder(gold, 1.0f, 1.0f, 16);

void renderObjects() {
	VertexOps::render(frameBuffer, board, lights, glm::mat3(1,0,0,0,1,0,0,0,1));
	VertexOps::render(frameBuffer, cone, lights, T(-2, 0, 0));
	VertexOps::render(frameBuffer, cylinderY, lights, T(2, 0, 0));
	VertexOps::render(frameBuffer, cylinderY, lights, T(-4, 0, 0) * Rx(M_PI_2));
	VertexOps::render(frameBuffer, plane, lights, T(0,0,-4));
}

static void render() {
	frameBuffer.clearColorAndDepthBuffers();
	int width = frameBuffer.getWindowWidth();
	int height = frameBuffer.getWindowHeight();

	float x, y, z;
	computeXYZFromAzimuthAndElevation(1.0f, az, el, x, y, z);
	glm::vec3 focusPoint = position + glm::vec3(x, y, z); // based on az, el, position, and angle
	VertexOps::viewingTransformation = glm::lookAt(position, focusPoint, Y_AXIS);

	float AR = (float)width / height;
	VertexOps::projectionTransformation = glm::perspective(float(glm::radians(60.0)), AR, 0.1f, 100.0f);
	VertexOps::setViewport(0, width - 1, 0, height - 1);
	renderObjects();
	frameBuffer.showColorBuffer();
}

void resize(int width, int height) {
	frameBuffer.setFrameBufferSize(width, height);
	float AR = (float)width / height;

	VertexOps::setViewport(0, width - 1, 0, height - 1);
	// VertexOps::projectionTransformation = glm::perspective(glm::radians(90.0), 2.0, 0.5, 50.0);

	VertexOps::projectionTransformation = glm::perspective(M_PI_3, AR, 0.5f, 80.0f);

	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
	const float INC = 0.5;
	switch (key) {
	case 'X':
	case 'x': theLight->lightPosition.x += (isupper(key) ? INC : -INC);
				std::cout << theLight->lightPosition << std::endl;
				break;
	case 'Y':
	case 'y': theLight->lightPosition.y += (isupper(key) ? INC : -INC);
				std::cout << theLight->lightPosition << std::endl;
				break;
	case 'Z':
	case 'z':	theLight->lightPosition.z += (isupper(key) ? INC : -INC);
				std::cout << theLight->lightPosition << std::endl;
				break;
	case 'P':
	case 'p':	isMoving = !isMoving;
				break;
	case 'C':	// Do something here
	case 'c':	angle = az;
				break;
	case '?':	twoViewOn = !twoViewOn;
				break;
	case ESCAPE:
		glutLeaveMainLoop();
		break;
	default:
		std::cout << (int)key << "unmapped key pressed." << std::endl;
	}

	glutPostRedisplay();
}

static void special(int key, int x, int y) {
	static const double rotateInc = glm::radians(10.0);
	static const double minEL = -glm::radians(80.0);
	static const double maxEL = glm::radians(80.0);
	double minAz = angle - glm::radians(90.0);
	double maxAz = angle + glm::radians(90.0);

	std::cout << key << std::endl;
	switch (key) {
	case(GLUT_KEY_LEFT):	az = glm::clamp(double(az + rotateInc), minAz, maxAz);
							break;
	case(GLUT_KEY_RIGHT):	az = glm::clamp(double(az - rotateInc), minAz, maxAz);
							break;
	case(GLUT_KEY_DOWN):	el = glm::clamp(double(el - rotateInc), minEL, maxEL);
							break;
	case(GLUT_KEY_UP):		el = glm::clamp(double(el + rotateInc), minEL, maxEL);
							break;
	}
	glutPostRedisplay();
}

static void timer(int id) {
	// You should change this.
	if (isMoving) {
		// angle += glm::radians(5.0);
		position += glm::vec3(sin(angle), 0.0f, cos(angle)) * SPEED;
	}
	glutTimerFunc(100, timer, 0);
	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	GLuint world_Window = glutCreateWindow(extractBaseFilename(__FILE__).c_str());
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutTimerFunc(100, timer, 0);
	glutMouseFunc(mouseUtility);

	frameBuffer.setClearColor(lightGray);

	glutMainLoop();

	return 0;
}
