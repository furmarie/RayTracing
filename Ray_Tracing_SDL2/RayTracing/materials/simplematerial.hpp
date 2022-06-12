#ifndef SIMPLEMATERIAL_H
#define SIMPLEMATERIAL_H

#include "materialbase.hpp"

namespace fRT {
	class simpleMaterial : public materialBase {
	public:
		// Constructor and destructor
		simpleMaterial();

		virtual ~simpleMaterial();

		// Function to return the colour
		virtual vec3 computeColour(
			const std::vector<std::shared_ptr<objectBase>>& objectList,
			const std::vector<std::shared_ptr<lightBase>>& lightList,
			hitRecord& record,
			const ray& cameraRay,
			int reflectionRayCount
		) override;
	// Function to compute specular highlights
		vec3 computeSpecular(
			const std::vector<std::shared_ptr<objectBase>>& objectList,
			const std::vector<std::shared_ptr<lightBase>>& lightList,
			hitRecord& record,
			const ray& cameraRay
		);

	public:
		vec3 m_baseColour{ {1.0, 0.0, 1.0} };
		double m_reflectivity = 0.0;
		double m_shininess = 0.0;
	};

};

#endif

