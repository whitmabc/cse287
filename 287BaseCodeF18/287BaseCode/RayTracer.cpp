#include "RayTracer.h"
#include "IShape.h"

/**
 * @fn	RayTracer::RayTracer(const color &defa)
 * @brief	Constructs a raytracers.
 * @param	defa	The clear color.
 */

RayTracer::RayTracer(const color &defa)
	: defaultColor(defa) {
}

/**
 * @fn	void RayTracer::raytraceScene(FrameBuffer &frameBuffer, int depth, const IScene &theScene) const
 * @brief	Raytrace scene
 * @param [in,out]	frameBuffer	Framebuffer.
 * @param 		  	depth	   	The current depth of recursion.
 * @param 		  	theScene   	The scene.
 */

void RayTracer::raytraceScene(FrameBuffer &frameBuffer, int depth,
								const IScene &theScene, const int aa = 1) const {
	const RaytracingCamera &camera = *theScene.camera;
	const std::vector<VisibleIShapePtr> &objs = theScene.visibleObjects;
	const std::vector<PositionalLightPtr> &lights = theScene.lights;

	for (int y = 0; y < frameBuffer.getWindowHeight(); ++y) {
		for (int x = 0; x < frameBuffer.getWindowWidth(); ++x) {
			DEBUG_PIXEL = (x == xDebug && y == yDebug);
			
			// Anti-Aliasing
			color aaColor = black;
			int aaOffset = aa / 2;
			for (int aaY = 0; aaY < aa; aaY++) {
				for (int aaX = 0; aaX < aa; aaX++) {
					float xPos = x + (-aaOffset + aaX) / (float)aa;
					float yPos = y + (-aaOffset + aaY) / (float)aa;

					Ray ray = camera.getRay((float)x, (float)y);
					aaColor += glm::clamp(traceIndividualRay(ray, theScene, depth), 0.0f, 1.0f);
				}
			}

			color colorForPixel = glm::clamp(aaColor * (1.0f / (aa * aa)), 0.0f, 1.0f);
			frameBuffer.setColor(x, y, colorForPixel);
		}
	}

	frameBuffer.showColorBuffer();

}

/**
 * @fn	color RayTracer::traceIndividualRay(const Ray &ray, const IScene &theScene, int recursionLevel) const
 * @brief	Trace an individual ray.
 * @param	ray			  	The ray.
 * @param	theScene	  	The scene.
 * @param	recursionLevel	The recursion level.
 * @return	The color to be displayed as a result of this ray.
 */

color RayTracer::traceIndividualRay(const Ray &ray, const IScene &theScene, int recursionLevel) const {
	HitRecord theHit = VisibleIShape::findIntersection(ray, theScene.visibleObjects);
	color result = defaultColor;


	if (theHit.t < FLT_MAX) {

		for (PositionalLight *light : theScene.lights) {
			// Shadows
			// origin point on object
			glm::vec3 shadowFeelerStart = theHit.interceptPoint + EPSILON * theHit.surfaceNormal;

			// shadow feeler ray, change to use every light
			Ray shadowFeeler = Ray(shadowFeelerStart, glm::normalize(light->lightPosition - shadowFeelerStart));

			HitRecord hit = VisibleIShape::findIntersection(shadowFeeler, theScene.visibleObjects);

			// Is the object's feeler to the light source obstructed by another object
			bool inShadow = (hit.t < glm::distance(light->lightPosition, theHit.interceptPoint));

			// Texturing
			if (theHit.texture != nullptr) {
				float u = glm::clamp(theHit.u, 0.0f, 1.0f);
				float v = glm::clamp(theHit.v, 0.0f, 1.0f);
				result += theHit.texture->getPixel(u, v);
			}
			else {
				if (DEBUG_PIXEL) {
					std::cout << std::endl;
				}
				// Compute color normally
				result = light->illuminate(theHit.interceptPoint, theHit.surfaceNormal, theHit.material, theScene.camera->cameraFrame, inShadow);
			}
		}
	}
	return glm::clamp(result, 0.0f, 1.0f);

}