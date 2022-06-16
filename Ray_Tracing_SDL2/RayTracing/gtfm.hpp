#ifndef GTFM_H
#define GTFM_H

#include "qbLinAlg/qbVector.h"
#include "qbLinAlg/qbMatrix.h"
#include "ray.hpp"

namespace fRT {
	// Define direction flag values
	constexpr bool FWDTFRM = true;  // Forward transform
	constexpr bool BCKTFRM = false; // Backward transform

	class GTform {
	public:
		// Constructor and Destructor
		GTform();
		~GTform();

		// Construct from a pair of matrices
		GTform(const matrix& fwd, const matrix& bck);

		// Construct from three vector
		GTform(
			const vec3& translation,
			const vec3& rotation,
			const vec3& scale
		);

		// Function to set translation, rotation and scale components
		// Make sure no component is zero in scale vector
		void setTransform(
			const vec3& translation,
			const vec3& rotation,
			const vec3& scale
		);

	// Functions to return transform matrices
		matrix getForward();
		matrix getBackward();

		// Function to apply the transform
		ray apply(const ray& inputRay, bool dirFlag) const;
		vec3 apply(const vec3& inputVec, bool dirFlag) const;

		// Overload operators
		friend GTform operator* (const fRT::GTform& lhs, const fRT::GTform& rhs);

		// Overload assignment operator
		GTform operator= (const GTform& rhs);

		// Function to print transform matrix
		void printMatrix(bool dirFlag) const;

		// Function to print vectors
		void printVector(vec3& v) const;

	private:
		void print(const matrix& mat) const;

	private:
		matrix m_fwdtfm{ 4, 4 };
		matrix m_bcktfm{ 4, 4 };
	};
};



#endif