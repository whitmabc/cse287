#include <ctime>
#include <vector>
#include "defs.h"
#include "Utilities.h"
#include "FrameBuffer.h"
#include "ColorAndMaterials.h"
#include "Rasterization.h"

FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

void closed5x5Square(int x, int y, color C) {
	drawLine(frameBuffer, x - 2, y, x + 2, y, C);
}

void closed5x5Square(const glm::vec2 &centerPt, color C) {
}

void open5x5Square(const glm::vec2 &centerPt, color C) {
}

void pieChart(const glm::vec2 &centerPt, float rad, float perc, const color &C1, const color &C2) {
	drawArc(frameBuffer, centerPt, rad, M_PI_4, M_PI, C1);
}

void render() {
	frameBuffer.clearColorAndDepthBuffers();
	closed5x5Square(50, 50, red);
	closed5x5Square(glm::vec2(100, 50), green);
	open5x5Square(glm::vec2(150, 50), blue);
	pieChart(glm::vec2(250, 100), 50, 0.25, red, green);
	frameBuffer.showColorBuffer();
}

void resize(int width, int height) {
	frameBuffer.setFrameBufferSize(width, height);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case ESCAPE:	glutLeaveMainLoop();
					break;
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	GLuint world_Window = glutCreateWindow(__FILE__);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseUtility);

	frameBuffer.setClearColor(black);

	glutMainLoop();

	return 0;
}