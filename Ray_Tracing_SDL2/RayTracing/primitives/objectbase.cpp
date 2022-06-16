#include "objectbase.hpp"

#include <math.h>

constexpr double EPSILON = 1e-21f;


namespace fRT {
	// Default constructor
	objectBase::objectBase() {

	}

	// Destructor
	objectBase::~objectBase() {}

	// Function to check intersection
	bool objectBase::checkIntersection(const ray& r,
		qbVector<double>& intPoint,
		qbVector<double>& localNormal,
		qbVector<double>& localColour
	) {
		return false;
	}

	// Function to set transform matrix
	void objectBase::setTransformMatrix(GTform transformMat) {
		m_transformMatrix = transformMat;
	}

	// Function to test whether two floating-points are close to being equal
	bool objectBase::isClose(const double f1, const double f2) const {
		return fabs(f1 - f2) < EPSILON;
	}

	// Function to assign a material 
	bool objectBase::assignMaterial(const std::shared_ptr<fRT::materialBase>& objMaterial) {
		m_pMaterial = objMaterial;
		m_hasMaterial = true;
		return m_hasMaterial;
	}
};