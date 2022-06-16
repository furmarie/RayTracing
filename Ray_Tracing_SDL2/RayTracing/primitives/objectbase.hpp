#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include "../qbLinAlg/qbVector.h"
#include "../ray.hpp"
#include "../gtfm.hpp"
#include "../../consts.h"

namespace fRT {
	// Forward declare the material base class
	// Will be overriden later
	class materialBase;

	class objectBase {
	public:
		// Constructor and Destructor
		objectBase();
		virtual ~objectBase();

		// Function to check intersection
		virtual bool checkIntersection(
			const ray& r,
			qbVector<double>& intPoint,
			qbVector<double>& localNormal,
			qbVector<double>& localColour
		);

		// Function to set the transform matrix
		void setTransformMatrix(GTform transformMat);

		// Function to test whether two floating-points are close to being equal
		bool isClose(const double f1, const double f2) const;

		// Function to assign a material to object
		bool assignMaterial(const std::shared_ptr<fRT::materialBase>& objMaterial);

	// Public variables
	public:
		// Base colour of object
		qbVector<double> m_baseColour{ 3 };

		// Geometric transform to be applied to the object
		GTform m_transformMatrix;

		// Reference to material of this object
		std::shared_ptr<fRT::materialBase> m_pMaterial;

		// Flag to indicate whether object has material or not
		bool m_hasMaterial = false;

	public:
		// Store the uv coordinates from a detected intersection
		qbVector<double> m_uvPoint {2};
	};
};

#endif

