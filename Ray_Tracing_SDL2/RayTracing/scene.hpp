#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <SDL.h>
#include "fImage.hpp"
#include "camera.hpp"
#include "primitives/sphere.hpp"
#include "primitives/plane.hpp"
#include "lights/pointlight.hpp"
#include "hitrecord.h"


namespace fRT {
	class Scene {
	public:
		// Default constructor
		Scene();

		// Function to actually render
		bool Render(fImage& outputImage);

		// Function to cast a ray into the scene
		bool castRay(
			ray& r,
			hitRecord& record
		);

		// Private functions
	private:

		// Private members:
	private:
		// Camera for this scene
		camera m_camera;

		// List of all objects in the scene
		std::vector<std::shared_ptr<objectBase>> m_world;

		// List of all lights in the scene
		std::vector<std::shared_ptr<lightBase>> m_lights;
	};
};


#endif