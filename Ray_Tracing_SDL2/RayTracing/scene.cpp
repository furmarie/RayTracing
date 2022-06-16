#include <chrono>
#include <thread>

#include "ctpl.h"
#include "scene.hpp"
#include "materials/simplematerial.hpp"
#include "textures/checker.hpp"


void printVec(const qbVector<double>& vec) {
	int nRows = vec.GetNumDims();
	for(int i = 0; i < nRows; i++) {
		std::cout << std::fixed << std::setprecision(3) << vec.GetElement(i) << " ";
	}
	std::cout << std::endl;
}




namespace fRT {
	// Default constructor
	Scene::Scene() {
		// Create some materials
		auto yellowDiffuse = std::make_shared<simpleMaterial>();
		auto blueDiffuse = std::make_shared<simpleMaterial>();
		auto goldMetal = std::make_shared<simpleMaterial>();
		auto testMaterial2 = std::make_shared<simpleMaterial>();
		auto testMaterial3 = std::make_shared<simpleMaterial>();
		auto floorMaterial = std::make_shared<simpleMaterial>();

		// Set material params
		blueDiffuse->m_baseColour = vec3({ 0.2, 0.2, 0.81 });
		blueDiffuse->m_reflectivity = 0.05;
		blueDiffuse->m_shininess = 5.0;

		yellowDiffuse->m_baseColour = vec3({ 0.8, 0.8, 0.2 });
		yellowDiffuse->m_reflectivity = 0.05;
		yellowDiffuse->m_shininess = 5.0;

		goldMetal->m_baseColour = vec3({0.8, 0.8, 0.3});
		goldMetal->m_reflectivity = 0.25;
		goldMetal->m_shininess = 20.0;


		floorMaterial->m_baseColour = vec3({ 1.0, 1.0, 1.0 });
		floorMaterial->m_reflectivity = 0.5;
		floorMaterial->m_shininess = 0.0;



		// Configure the camera
		m_camera.setPosition(qbVector<double>{std::vector<double> {0, -17, -4}});
		m_camera.setLookAt(qbVector<double>{std::vector<double> {0.0, 0.0, 0.0}});
		m_camera.setUp(qbVector<double>{std::vector<double> {0.0, 0.0, 1.0}});
		m_camera.setHorzSize(0.25);
		m_camera.setAspectRatio(16.0 / 9.0);
		m_camera.UpdateCameraGeometry();

		// Create some textures
		auto floorTexture = std::make_shared<texture::checker>();
		floorTexture->setTransform(vec3({0.0, 0.0}), 0.0, vec3({10.0, 10.0}));

		// Assign to material
		floorMaterial->assignTexture(floorTexture);
		yellowDiffuse->assignTexture(floorTexture);

		// Construct a test cylinder
		auto cylinder1 = std::make_shared<cylinder>();

		// Define a transform for cylinder
		GTform testMatCyl;
		testMatCyl.setTransform(
			vec3({ -1, 0.0, 0.0 }),
			vec3({ -PI / 4,0, 0.0 }),
			vec3({ 0.5, 0.5, 1.0 })
		);
		//cylinder1->setTransformMatrix(testMatCyl);
		cylinder1->setTransformMatrix(
			GTform(
				vec3({ 0, 0, -0.6 }),
				vec3({ -PI / 4, -PI / 4, 0.0 }),
				vec3({ 0.4, 0.4, 3.0 })
			)
		);
		cylinder1->assignMaterial(blueDiffuse);

		m_world.push_back(cylinder1);

		auto cone1 = std::make_shared<cone>();
		cone1->setTransformMatrix(
			GTform(
				vec3({ 1, 0, 0 }),
				vec3({ 0.0, 0, 0 }),
				vec3({ 0.5, 0.5, 1 })
			)
		);
		cone1->assignMaterial(yellowDiffuse);

		//m_world.push_back(cone1);

		//m_world.push_back(std::make_shared<sphere>());
		//m_world[0]->assignMaterial(blueDiffuse);

		// Create a floor
		auto floor = std::make_shared<plane>();
		auto bb = GTform(vec3({ 0.0, 0.0, 1.0 }), vec3({ 0.0, 0.0, 0.0 }), vec3({ 16, 16, 1 }));
		floor->setTransformMatrix(bb);
		floor->assignMaterial(floorMaterial);
		

		m_world.push_back(floor);

		// Create a torus
		auto torus1 = std::make_shared<torus>();
		torus1->setTransformMatrix(
			GTform(
				vec3({ 0, 0, -0.6 }),
				vec3({ -PI / 4, -PI/4, 0.0 }),
				vec3({ 1, 1, 1 })
			)
		);
		torus1->m_baseColour = vec3({1.0,1.0,1.0});
		torus1->assignMaterial(goldMetal);

		m_world.push_back(torus1);


		// Test light
		m_lights.push_back(std::make_shared<pointLight>(pointLight()));
		m_lights.at(0)->m_location = qbVector<double>({ -5.0, -10.0, -5.0 });
		m_lights.at(0)->m_colour = qbVector<double>({ 1.0, 1.0, 1.0 });

		m_lights.push_back(std::make_shared<pointLight>(pointLight()));
		m_lights.at(1)->m_location = vec3({ 5, -10, -5 });
		m_lights.at(1)->m_colour = qbVector<double>({ 1.0, 1.0, 1.0 });

		m_lights.push_back(std::make_shared<pointLight>(pointLight()));
		m_lights.at(2)->m_location = vec3({ 0, -10, -5 });
		m_lights.at(2)->m_colour = qbVector<double>({ 1.0, 1.0, 1.0 });

	}

	// Function to actually render
	bool Scene::Render(fImage& outputImage) {
		// Create a thread pool
		ctpl::thread_pool threads(1);


		// Get dimensions of image
		int xSize = outputImage.getxSize();
		int ySize = outputImage.getySize();

		// Check each pixel in image and send ray out to that
		// and check if there is an intersection
		ray cameraRay;

		double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
		double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);
		double minDist = 1e6;
		double maxDist = 0.0;

		std::cerr << m_world.size() << std::endl;

		std::cerr << "RENDERING IMAGE" << std::endl;
		auto start_time = std::chrono::high_resolution_clock::now();

		// Function to run for every pixel x, y
		auto pixelFunc = [&](int id, int x, int y) {
			//std::cerr << "Process id: " << id << std::endl;
			// Normalize the x and y coordinates.
			double normX = (static_cast<double>(x) * xFact) - 1.0;
			double normY = (static_cast<double>(y) * yFact) - 1.0;

			// Generate the ray for this pixel.
			m_camera.generateRay(normX, normY, cameraRay);

			// Hold records of closest hit object
			hitRecord record;

			bool intersectionFound = castRay(cameraRay, record);

			// Compute illumination for closest object if intersectionFound
			if(intersectionFound) {
				// Check if the closest object has a material
				if(record.obj->m_hasMaterial) {
					// Use the material to compute the colour
					vec3 colour = record.obj->m_pMaterial->computeColour(
						m_world, m_lights, record, cameraRay, 0
					);
					outputImage.SetPixel(x, y, colour[0], colour[1], colour[2]);
				}
				else {
					// Use basic method to compute colour
					vec3 matColour = materialBase::computeDiffuseColour(m_world, m_lights, record, record.obj->m_baseColour);
					outputImage.SetPixel(x, y, matColour[0], matColour[1], matColour[2]);
				}
			}
		};

		auto do_line = [&](int id, int y) -> void {
			for(int x = 0; x < xSize; x++) {
				pixelFunc(id, x, y);
			}
		};

		auto do_length = [&](int id, int strt, int en) {
			for(int y = strt; y <= en; y++) {
				do_line(id, y);
			}
		};

		std::vector<std::thread> thread_list;

		int stepsize = 50;
		//for(int i = 0; i < ySize; i += stepsize) {
		//	do_length(1, i, std::min(ySize - 1, i + stepsize - 1));
		//	//thread_list.push_back(std::thread(do_length, 1, i, std::min(ySize - 1, i + stepsize - 1)));
		//	//threads.push(do_length, i, std::min(ySize - 1, i + stepsize - 1));
		//}

		//for(auto& th : thread_list) {
		//	th.join();
		//}

		for(int y = 0; y < ySize; y++) {
			std::cerr << "Lines done :" << y + 1 << "/" << ySize << std::endl;
			for(int x = 0; x < xSize; x++) {
				// Push function call to thread list
				threads.push(pixelFunc, x, y);

			}

			// Push line to thread pool
			//threads.push(do_line, y);
		}
		// Wait for all calls to finish
		threads.stop(true);



		auto end_time = std::chrono::high_resolution_clock::now();
		std::cerr << "\nTIME TAKEN: " << " ";
		std::cerr << std::fixed << std::setprecision(3) << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() * 1e-3 << " s\n" << std::endl;


		//std::cerr << "Min distance: " << minDist << std::endl;
		//std::cerr << "Max distance: " << maxDist << std::endl;
		return true;
	}
	// Function to cast ray into the scene
	bool Scene::castRay(
		ray& r,
		hitRecord& record
	) {
		// Test for intersection with all objects in the scene
		vec3 intPoint{ 3 };
		vec3 localNormal{ 3 };
		vec3 localColour{ 3 };
		double minDist = 1e6;
		bool intersectionFound = false;

		for(int i = 0; i < m_world.size(); i++) {
			auto& currObj = m_world[i];
			bool hits = currObj->checkIntersection(r, intPoint, localNormal, localColour);

			// If ray intersects, return colour
			if(hits) {
				// Set flag to indicate that intersection is found
				intersectionFound = true;

				// Compute distance between camera and point of intersection
				double dist = (intPoint - r.m_point1).norm();

				// If this object is closer than any we have seen till now
				// record this as closest
				if(dist < minDist) {
					minDist = dist;
					record.intPoint = intPoint;
					record.localNormal = localNormal;
					record.localColour = localColour;
					record.obj = currObj;
				}
			}
		} // Done checking the world for intersection
		return intersectionFound;
	}
}

