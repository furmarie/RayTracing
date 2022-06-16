#ifndef MATERIALBASE_H
#define MATERIALBASE_H

#include <memory>
#include "../primitives/objectbase.hpp"
#include "../lights/lightbase.hpp"
#include "../textures/texturebase.hpp"
#include "../qbLinAlg/qbVector.h"
#include "../ray.hpp"
#include "../hitrecord.h"

namespace fRT {
	class materialBase {
	public:
		// Constructor and destructor
		materialBase();
		virtual ~materialBase();

		// Function to return colour of the material
		virtual vec3 computeColour(
			const std::vector<std::shared_ptr<objectBase>>& objectList,
			const std::vector<std::shared_ptr<lightBase>>& lightList,
			hitRecord& record,
			const ray& cameraRay,
			int reflectionRayCount
		);

		// Function to compute diffuse colour
		static vec3 computeDiffuseColour(
			const std::vector<std::shared_ptr<objectBase>>& objectList,
			const std::vector<std::shared_ptr<lightBase>>& lightList,
			hitRecord& record,
			const vec3& baseColour
		);

		// Function to compute the reflection colour
		vec3 computeReflectionColour(
			const std::vector<std::shared_ptr<objectBase>>& objectList,
			const std::vector<std::shared_ptr<lightBase>>& lightList,
			hitRecord record,
			const ray& incidentRay,
			int reflectionRayCount
		);

		// Function to cast a ray into scene
		bool castRay(
			const ray& r,
			const std::vector<std::shared_ptr<objectBase>>& objectList,
			const std::shared_ptr<objectBase>& thisObj,
			hitRecord& record
		);

		// Function to assign a texture
		void assignTexture(const std::shared_ptr<texture::textureBase>& tex);

	public:
		// Counter for number of reflections rays
		inline static int m_maxReflectionRays;
		inline static int m_reflectionRayCount;

		// List of textures assigned to this material
		std::vector<std::shared_ptr<texture::textureBase>> m_textureList;

		// Flag to indicate whether at least one texture has been assigned
		bool m_hasTexture = false;

	private:


	};

};

#endif

