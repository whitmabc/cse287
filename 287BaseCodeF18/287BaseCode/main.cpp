#include <ctime>
#include <vector>
#include "defs.h"
#include "Utilities.h"
#include "FrameBuffer.h"
#include "ColorAndMaterials.h"

int main(int argc, char *argv[]) {
	std::cout << glm::cos(0.0f) << std::endl;
	std::cout << std::cos(0.0f) << std::endl;

	std::cout << glm::degrees(M_PI_2) << std::endl;

	std::cout << "hello world" << std::endl;
	float x = 10;
	float y = 20;
	swap(x, y);
	return 0;
}