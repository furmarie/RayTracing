#include "materialbase.hpp"

namespace fRT {
	// Constructor and Destructor
	materialBase::materialBase() {
		m_maxReflectionRays = 10;
		m_reflectionRayCount = 0;
	}

	materialBase::~materialBase() {}

	// Function to compute colour of material
	vec3 materialBase::computeColour(
		const std::vector<std::shared_ptr<objectBase>>& objectList,
		const std::vector<std::shared_ptr<lightBase>>& lightList,
		hitRecord& record,
		const ray& cameraRay,
		int reflectionRayCount
	) {
		// Define initial material colour
		vec3 matColour{ 3 };
		return matColour;
	}

	// Function to compute diffuse colour
	vec3 materialBase::computeDiffuseColour(
		const std::vector<std::shared_ptr<objectBase>>& objectList,
		const std::vector<std::shared_ptr<lightBase>>& lightList,
		hitRecord& record,
		const vec3& baseColour
	) {
		// Compute the colour due to diffuse illumination
		vec3 diffuseColour{ 3 };
		double intensity = 0.0;
		vec3 colour{ 3 };
		double red = 0, green = 0, blue = 0;
		bool validIllum = false;
		bool illumFound = false;

		for(auto& currLight : lightList) {
			validIllum = currLight->computeIllumination(
				record.intPoint,
				record.localNormal,
				objectList,
				record.obj,
				colour,
				intensity
			);
			if(validIllum) {
				illumFound = true;
				red += colour[0] * intensity;
				green += colour[1] * intensity;
				blue += colour[2] * intensity;
			}
		}

		if(illumFound) {
			// Set diffuse colour
			diffuseColour[0] = red * baseColour[0];
			diffuseColour[1] = green * baseColour[1];
			diffuseColour[2] = blue * baseColour[2];
		}
		// Return colour of material
		return diffuseColour;
	}


	// Function to compute colour due to reflection
	vec3 materialBase::computeReflectionColour(
		const std::vector<std::shared_ptr<objectBase>>& objectList,
		const std::vector<std::shared_ptr<lightBase>>& lightList,
		hitRecord record,
		const ray& incidentRay,
		int reflectionRayCount
	) {
		vec3 reflectedColour {3};

		// Compute the reflection vector
		vec3 d = incidentRay.m_AB;
		vec3 reflectedVector = d - 2 * (vec3::dot(d, record.localNormal)) * record.localNormal;

		// Construct the reflection ray
		vec3 startPoint = record.intPoint;
		ray reflectedRay (startPoint, startPoint + reflectedVector);

		// Cast this ray into the scene and find the closest object that it intersects with
		hitRecord closestIntersectionRec;

		bool intersectionFound = castRay(
			reflectedRay,
			objectList,
			record.obj,
			closestIntersectionRec
		);

		// Compute illumination for closest object if there was a valid intersection
		vec3 matColour {3};
		if(intersectionFound and (reflectionRayCount < m_maxReflectionRays)) {
			// Check if closest object has a material 
			if(closestIntersectionRec.obj->m_hasMaterial) {
				// Use material to compute the colour
				matColour = closestIntersectionRec.obj->m_pMaterial->computeColour(
					objectList, lightList, closestIntersectionRec, reflectedRay, reflectionRayCount + 1
				);
			}
			else {
				matColour = materialBase::computeDiffuseColour(
					objectList, lightList, closestIntersectionRec, closestIntersectionRec.obj->m_baseColour
				);
			}
		}
		else {
			// Leave matColour as it is
		}
		reflectedColour = matColour;

		return reflectedColour;
	}

	// Function to cast a ray into the scene
	bool materialBase::castRay(
		const ray& r,
		const std::vector<std::shared_ptr<objectBase>>& objectList,
		const std::shared_ptr<objectBase>& thisObj,
		hitRecord& record
	) {
		// Test for intersections with all of the objects in the scene
		vec3 intPoint {3};
		vec3 localNormal {3};
		vec3 localColour {3};

		double minDist = 1e6;
		bool intersectionFound = false;
		for(auto& currObj : objectList) {
			if(currObj != thisObj) {
				bool validInt = currObj->checkIntersection(
					r, intPoint, localNormal, localColour
				);
				if(validInt) {
					// Set flag for found intersection
					intersectionFound = true;

					// Compute the distance between source and intersection point
					double dist = (intPoint - r.m_point1).norm();
					// Store this objecy in record if it is the closest
					if(dist < minDist) {
						minDist = dist;
						record.intPoint = intPoint;
						record.localNormal = localNormal;
						record.localColour = localColour;
						record.obj = currObj;
					}
				}
			}
		}// Done checking object list for intersection

		return intersectionFound;
	}

	// Function to assign a texture

	void materialBase::assignTexture(const std::shared_ptr<texture::textureBase>& tex) {
		m_textureList.push_back(tex);
		m_hasTexture = true;
	}
};