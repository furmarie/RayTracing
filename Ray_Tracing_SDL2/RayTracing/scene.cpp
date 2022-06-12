#include <chrono>
#include <thread>

#include "ctpl.h"
#include "scene.hpp"
#include "materials/simplematerial.hpp"


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
		auto testMaterial1 = std::make_shared<simpleMaterial>();
		auto testMaterial2 = std::make_shared<simpleMaterial>();
		auto testMaterial3 = std::make_shared<simpleMaterial>();
		auto floorMaterial = std::make_shared<simpleMaterial>();

		// Set material params
		testMaterial1->m_baseColour = vec3({ 0.35, 0.9, 0.8 });
		testMaterial1->m_reflectivity = 0.1;
		testMaterial1->m_shininess = 10.0;

		testMaterial2->m_baseColour = vec3({ 0.2, 0.8, 0.3 });
		testMaterial2->m_reflectivity = 0.75;
		testMaterial2->m_shininess = 10.0;

		testMaterial3->m_baseColour = vec3({ 1.0, 0.8, 0.0});
		testMaterial3->m_reflectivity = 0.25;
		testMaterial3->m_shininess = 10.0;
		
		floorMaterial->m_baseColour = vec3({1.0, 1.0, 1.0});
		floorMaterial->m_reflectivity = 0.5;
		floorMaterial->m_shininess = 0.0;



		// Configure the camera
		m_camera.setPosition(qbVector<double>{std::vector<double> {0, -10, -1}});
		m_camera.setLookAt(qbVector<double>{std::vector<double> {0.0, 0.0, 0.0}});
		m_camera.setUp(qbVector<double>{std::vector<double> {0.0, 0.0, 1.0}});
		m_camera.setHorzSize(0.25);
		m_camera.setAspectRatio(16.0 / 9.0);
		m_camera.UpdateCameraGeometry();

		const double mx = 255.0;

		// Test sphere
		m_world.push_back(std::make_shared<sphere>());
		m_world.push_back(std::make_shared<sphere>());
		m_world.push_back(std::make_shared<sphere>());

		// Construct a test plane
		m_world.push_back(std::make_shared<plane>());
		m_world.at(3)->m_baseColour = vec3({ 1,1,1 });

		 //Define transform for plane
		GTform planeMat;
		planeMat.setTransform(
			vec3({ 0, 0, 0.7 }),
			vec3({ 0, 0.0, 0 }),
			vec3({ 4.0, 4.0, 4.0 })
		);
		m_world.at(3)->setTransformMatrix(planeMat);

		// Modify the spheres
		GTform testMat1, testMat2, testMat3;
		testMat1.setTransform(
			vec3({ -1.5, 0, 0 }),
			vec3({ 0, 0.0, 0.0 }),
			vec3({ 0.5, 0.5, 0.5 })
		);
		testMat2.setTransform(
			vec3({ 0.0, 0.0, 0.0 }),
			vec3({ 0.0, 0.0, 0.0 }),
			vec3({ 0.5, 0.5, 0.5 })
		);
		testMat3.setTransform(
			vec3({ 1.5, 0, 0 }),
			vec3({ 0, 0, 0 }),
			vec3({ 0.5, 0.5, 0.5 })
		);
		m_world.at(0)->setTransformMatrix(testMat1);
		m_world.at(1)->setTransformMatrix(testMat2);
		m_world.at(2)->setTransformMatrix(testMat3);

		m_world.at(0)->m_baseColour = vec3({ 0.3, 0.9, 0.85 });
		m_world.at(1)->m_baseColour = vec3({ 0.8, 0.2, 0.6 });
		m_world.at(2)->m_baseColour = vec3({ 0.8, 0.4, 0.2 });

		// Assign material to objects
		m_world.at(0)->assignMaterial(testMaterial2);
		m_world.at(1)->assignMaterial(testMaterial1);
		m_world.at(2)->assignMaterial(testMaterial3);
		m_world.at(3)->assignMaterial(floorMaterial);

		// Test light
		m_lights.push_back(std::make_shared<pointLight>(pointLight()));
		m_lights.at(0)->m_location = qbVector<double>({ -5.0, -10.0, -4.0 });
		m_lights.at(0)->m_colour = qbVector<double>({ 1.0, 0.0, 0.0 });

		m_lights.push_back(std::make_shared<pointLight>(pointLight()));
		m_lights.at(1)->m_location = vec3({ 5, -10, -4 });
		m_lights.at(1)->m_colour = qbVector<double>({ 0.0, 1.0, 0.0 });

		m_lights.push_back(std::make_shared<pointLight>(pointLight()));
		m_lights.at(2)->m_location = vec3({ 0, -10, -4 });
		m_lights.at(2)->m_colour = vec3({ 0.0, 0.0, 1.0 });

	}

	// Function to actually render
	bool Scene::Render(fImage& outputImage) {
		// Create a thread pool
		ctpl::thread_pool threads(6);
		

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
					vec3 matColour = materialBase::computeDiffuseColour(m_world, m_lights, record);
					outputImage.SetPixel(x, y, matColour[0], matColour[1], matColour[2]);

				}
			}
		};

		auto do_line = [&](int id, int y) -> void {
			for(int x = 0; x < xSize; x++) {
				pixelFunc(1, x, y);
			}
		};


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

