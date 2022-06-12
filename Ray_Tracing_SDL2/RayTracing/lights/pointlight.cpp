#include "pointlight.hpp"

void printVec1(const qbVector<double>& vec) {
	int nRows = vec.GetNumDims();
	for(int i = 0; i < nRows; i++) {
		std::cout << std::fixed << std::setprecision(3) << vec.GetElement(i) << " ";
	}
	std::cout << std::endl;
}

namespace fRT {
	// Constructor
	pointLight::pointLight() {
		m_colour = qbVector<double>({ 1.0, 1.0, 1.0 });
		m_intensity = 1.0;
	}

	// Destructor
	pointLight::~pointLight() {}

	// Function to compute illumination
	bool pointLight::computeIllumination(
		const qbVector<double>& intPoint,
		const qbVector<double>& localNormal,
		const std::vector<std::shared_ptr<objectBase>>& objectList,
		const std::shared_ptr<objectBase>& currObj,
		qbVector<double>& colour,
		double& intensity
	) {
		// Construct a vector pointing from intersection point to light
		qbVector<double> lightDir = (m_location - intPoint).Normalized();

		// Compute a starting point
		qbVector<double> startPoint = intPoint;

		// Construct a ray from point of intersection to light
		ray lightRay(startPoint, startPoint + lightDir);

		// Check for intersections with all other objects in the scene
		vec3 poi{ 3 };
		vec3 poiNormal{ 3 };
		vec3 poiColour{ 3 };
		bool validInt = false;
		double lightDist = (m_location - startPoint).norm();

		for(auto& sceneObj : objectList) {
			if(sceneObj != currObj) {
				validInt = sceneObj->checkIntersection(lightRay, poi, poiNormal, colour);

				if(validInt) {
					double dist = (poi - startPoint).norm();
					if(dist > lightDist) {
						validInt = false;
					}
				}

				// If there is an intersection, no need to check further
				// That means this object is blocking the light, i.e casting a shadow
				// on the object on which we are checking for illumination
				if(validInt) {
					break;
				}
			}
		}

		// Continue to compute illumination if the light ray did not intersect with any
		// other objects in the scene, i.e no other objects are casting a shadow on this

		if(not validInt) {
			// Compute angle between local normal and light ray
			// Note: We assume local normal is a unit vector (localNormal is of object
			// whose illumination we are checking
			double theta = acos(vec3::dot(localNormal, lightDir));

			// If the normal is pointing away from light, then there is no illumination
			if(theta > PI / 2) {
				colour = m_colour;
				intensity = 0.0;
				return false;
			}
			else {
				// There is illumination
				colour = m_colour;
				intensity = m_intensity * (1.0 - (theta / (PI / 2)));
				return true;
			}
		}
		else {
			// Shadow is being cast, no illumination
			colour = m_colour;
			intensity = 0.0;
			return false;
		}
		return false;
	}



};
