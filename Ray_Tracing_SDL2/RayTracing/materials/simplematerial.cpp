#include "simplematerial.hpp"
#include <math.h>
namespace fRT {
	simpleMaterial::simpleMaterial() {}
	simpleMaterial::~simpleMaterial() {}


	// Function to return the colour
	vec3 simpleMaterial::computeColour(
		const std::vector<std::shared_ptr<objectBase>>& objectList,
		const std::vector<std::shared_ptr<lightBase>>& lightList,
		hitRecord& record, const ray& cameraRay,
		int reflectionRayCount
	) {
		// Define initial material colours
		vec3 matColour{ 3 };
		vec3 refColour{ 3 };
		vec3 difColour{ 3 };
		vec3 spcColour{ 3 };

		// Compute diffuse component
		difColour = computeDiffuseColour(
			objectList,
			lightList,
			record
		);

		// Compute the specular component
		if(m_shininess > 0.0) {
			spcColour = computeSpecular(
				objectList,
				lightList,
				record,
				cameraRay
			);
		}

		// Compute the reflective component
		if(m_reflectivity > 0.0) {
			refColour = computeReflectionColour(
				objectList, 
				lightList,
				record,
				cameraRay,
				reflectionRayCount + 1
			);
		}

		// Combine reflective and diffused component
		matColour = (refColour * m_reflectivity) + (difColour * (1 - m_reflectivity));

		// Combine specular component with final colour
		matColour = matColour + spcColour;
		double mx_val = fmax(fmax(matColour[0], matColour[1]), matColour[2]);
		//matColour = (1 / mx_val) * matColour;
		return matColour;
	}

	// Function to compute specular highlights
	vec3 simpleMaterial::computeSpecular(
		const std::vector<std::shared_ptr<objectBase>>& objectList,
		const std::vector<std::shared_ptr<lightBase>>& lightList,
		hitRecord& record,
		const ray& cameraRay
	) {
		vec3 spcColour{ 3 };
		double red = 0, green = 0, blue = 0;

		// Loop through all the lights in the scene
		for(auto& currLight : lightList) {
			// Check for intersections with all objects in the scene
			double intensity = 0.0;

			// Construct a vector from intersection point to the light
			vec3 lightDir = (currLight->m_location - record.intPoint).Normalized();

			// Compute a start point
			vec3 startPoint = record.intPoint + (lightDir * 0.001);

			// Construct a ray from the point of intersection to the light
			ray lightRay(startPoint, startPoint + lightDir);

			// Loop through all objects in the scene to check if any object
			// obstructs light from this source
			vec3 poi{ 3 };
			vec3 poiNormal{ 3 };
			vec3 poiColour{ 3 };
			bool validInt = false;
			for(auto& sceneObj : objectList) {
				if(sceneObj == record.obj) {
					continue;
				}
				validInt = (sceneObj->checkIntersection(lightRay,
					poi,
					poiNormal,
					poiColour)
					);

				// If no intersections were found, proceed with calculating
				// specular component
				if(validInt) {
					break;
				}

				// Compute reflection vector
				vec3 d = lightRay.m_AB;
				vec3 r = d - (2 * vec3::dot(d, record.localNormal) * record.localNormal);
				r.Normalize();

				// Compute the dot product
				vec3 v = cameraRay.m_AB;
				v.Normalize();
				double dotProduct = vec3::dot(r, v);

				// Only proceed if dot product is positive
				// i.e viewing angle and reflected ray are in same direction
				if(dotProduct > 0.0) {
					intensity = m_reflectivity * std::pow(dotProduct, m_shininess);
				}
			} // Done with object list
			red += currLight->m_colour[0] * intensity;
			green += currLight->m_colour[1] * intensity;
			blue += currLight->m_colour[2] * intensity;
		} // Done checking all lights
		spcColour[0] = red;
		spcColour[1] = blue;
		spcColour[2] = green;
		return spcColour;
	}

};
