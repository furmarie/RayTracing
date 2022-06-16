#ifndef TEXTUREBASE_H
#define TEXTUREBASE_H

#include <memory>
#include "../qbLinAlg/qbMatrix.h"
#include "../qbLinAlg/qbVector.h"
#include "../ray.hpp"


namespace fRT {
	namespace texture {
		class textureBase {
		public:
			// Default constructor and destructor
			textureBase();

			virtual ~textureBase();

			// Function to return colour at given point in
			// uv coordinate system
			// Note that colour is defined as a 4 dimensional vector
			virtual qbVector<double> getColour(vec3& uvPoint);

			// Function to set transform
			void setTransform(
				const qbVector<double>& translation,
				const double& rotation,
				const qbVector<double>& scale
				);

			// Function to blend RGBA colours, returning a RGB result
			static vec3 blendColours(const std::vector<qbVector<double>>& inputColourList);

			// Function to appply local transform to given input vector
			// Input is 2-D
			qbVector<double> applyTransform(const qbVector<double>& inputVector);

		private:
			// Initialise transform matrix to identity matrix
			matrix m_transformMatrix {3, 3, std::vector<double>{1, 0, 0, 0, 1, 0, 0, 0, 1}};
		};
	};
};


#endif