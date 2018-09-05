#include <ctime>
#include <vector>
#include "defs.h"
#include "Utilities.h"
#include "FrameBuffer.h"
#include "ColorAndMaterials.h"

int main(int argc, char *argv[]) {

	std::cout << "Testing swap, x = 10 y = 20 before swap" << std::endl;
	float x = 10;
	float y = 20;
	swap(x, y);
	std::cout << "X = " << x << " Y = " << y << std::endl;

	std::cout << "Testing approximatelyEqual, a = 1.000 b = 0.999" << std::endl;
	float a = 1.000f;
	float b = 0.999f;
	std::cout << "Expected: 1 (True)\nActual: " << approximatelyEqual(a, b) << std::endl;

	std::cout << "Testing approximatelyZero, a = 0.0000001" << std::endl;
	a = 0.0000001f;
	std::cout << "Expected: 1 (True)\nActual: " << approximatelyZero(a) << std::endl;

	std::cout << "Testing normalizeDegrees, degrees = 721" << std::endl;
	float degrees = 721;
	std::cout << "Expected: 1\nActual: " << normalizeDegrees(degrees) << std::endl;

	std::cout << "Testing normalizeRadians, radians = 3PI" << std::endl;
	float radians = M_2PI + M_PI;
	std::cout << "Expected: Pi (3.14159)\nActual: " << normalizeRadians(radians) << std::endl;

	std::cout << "Testing rad2deg, rads = PI" << std::endl;
	float rads = M_PI;
	std::cout << "Expected: 180\nActual: " << rad2deg(rads) << std::endl;

	std::cout << "Testing deg2rad, degs = 180" << std::endl;
	float degs = 180;
	std::cout << "Expected: PI (3.14159)\nActual: " << deg2rad(degs) << std::endl;

	std::cout << "Testing min, a = 3, b = 2, c = 10" << std::endl;
	a = 3;
	b = 2;
	float c = 10;
	std::cout << "Expected: 2\nActual: " << min(a, b, c) << std::endl;

	std::cout << "Testing max, a = 3, b = 2, c = 10" << std::endl;
	c = 10;
	std::cout << "Expected: 10\nActual: " << max(a, b, c) << std::endl;

	std::cout << "Testing pointOnUnitCircle, angleRads = PI, x = 0, y = 0" << std::endl;
	float angleRads = M_PI;
	x = 0;
	y = 0;
	pointOnUnitCircle(angleRads, x, y);
	std::cout << "Expected: x = -1, approximatelyZero(y) = 1 Actual: x = " << x << "\ny is zero: " << approximatelyZero(y) << std::endl;

	std::cout << "Testing areaOfTriangle, a = 4, b = 13, c = 15" << std::endl;
	a = 4;
	b = 13;
	c = 15;
	std::cout << "Expected: 24\nActual: " << areaOfTriangle(a, b, c) << std::endl;
	return 0;
}