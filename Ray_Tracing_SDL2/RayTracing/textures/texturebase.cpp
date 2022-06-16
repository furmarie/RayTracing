#include "texturebase.hpp"
#include <cmath>

namespace fRT {
	// Constructor and destructor
	texture::textureBase::textureBase() {}

	texture::textureBase::~textureBase() {}

	// Function to return colour at a given uv point
	qbVector<double> texture::textureBase::getColour(vec3& uvPoint) {
		// Setup output vector
		qbVector<double> outColour{ 4 };

		return outColour;
	}

	// Function to set the transform matrix
	void texture::textureBase::setTransform(
		const qbVector<double>& translation,
		const double& rotation,
		const qbVector<double>& scale
	) {
		// Build the transform matrix
		matrix rotationMatrix{ 3 , 3,
			std::vector<double>{
				cos(rotation), -sin(rotation), 0.0,
				sin(rotation), cos(rotation), 0.0,
				0.0, 0.0, 1.0
			}
		};

		matrix scaleMatrix{ 3, 3,
				std::vector<double>{
				scale[0], 0.0, 0.0,
				0.0, scale[1], 0.0,
				0.0, 0.0, 1.0
			}
		};

		matrix translationMatrix{ 3, 3,
				std::vector<double>{
				1.0, 0.0, translation[0],
				0.0, 1.0, translation[1],
				0.0, 0.0, 1.0
			}
		};

		// Combine to form final transform matrix
		m_transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	}

	// Function to blend colours
	vec3 texture::textureBase::blendColours(const std::vector<qbVector<double>>& inputColourList) {
		// Setup output colour
		vec3 outColour {3};

		return outColour;
	}

	// Function to apply the transform
	qbVector<double> texture::textureBase::applyTransform(const qbVector<double>& inputVector) {
		// Copy input vector and make it three elements
		vec3 newInput {3};
		newInput[0] = inputVector[0];
		newInput[1] = inputVector[1];
		
		// Apply the transform
		qbVector<double> res = m_transformMatrix * newInput;
		res.resize(2);

		return res;
	}

};