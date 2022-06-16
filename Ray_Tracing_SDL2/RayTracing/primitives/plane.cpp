#include "plane.hpp"
#include <math.h>

void printVec2(const qbVector<double>& vec) {
	int nRows = vec.GetNumDims();
	for(int i = 0; i < nRows; i++) {
		std::cout << std::fixed << std::setprecision(3) << vec.GetElement(i) << " ";
	}
	std::cout << std::endl;
}
namespace fRT {
	// Default constructor
	plane::plane() {}

	// Destructor
	plane::~plane() {}

	// Function to check for intersection
	bool plane::checkIntersection(
		const ray& r,
		qbVector<double>& intPoint,
		qbVector<double>& localNormal,
		qbVector<double>& localColour
	) {
		// Copy the ray and apply backwards transform
		ray bckRay = m_transformMatrix.apply(r, fRT::BCKTFRM);

		// Copy the m_AB vector from r and normalize it
		vec3 k = bckRay.m_AB.Normalized();

		// Check if there is an intersection, ray is not parallel to plane
		if(isClose(k[2], 0.0)) {
			// No intersection
			return false;
		}

		// There is an intersection
		double t = - (bckRay.m_point1[2] / k[2]);

		// If t < 0, intersection point is behind the camera and can be ignored
		if(t < 0.0) {
			return false;
		}

		// Compute values for u and v
		double u = bckRay.m_point1[0] + t * k[0];
		double v = bckRay.m_point1[1] + t * k[1];

		/* 
		   We take the plane to be the XY plane centered at origin of unit length and width
		   Transformation matrix will allow us to transform this plane as per requirement
		*/

		// If magnitudues of both u and v are less than 1, point is on the plane
		if(fabs(u) >= 1.0 or fabs(v) >= 1.0) {
			return false;
		}

		vec3 poi = bckRay.m_point1 + t * k;

		// Transform point of intersection back to world coordinates
		intPoint = m_transformMatrix.apply(poi, fRT::FWDTFRM);

		// Compute the localNormal
		vec3 localOrigin({0.0, 0.0, 0.0});
		vec3 normalVector({0.0, 0.0, -1.0});
		vec3 globalOrigin = m_transformMatrix.apply(localOrigin, fRT::FWDTFRM);

		localNormal = m_transformMatrix.apply(normalVector, fRT::FWDTFRM) - globalOrigin;
		//printVec2(localNormal);
		localNormal.Normalize();

		// Rotate localNormal if direction of castRay and localNormal is same
		// For plane, castRay will never hit a side which is not to be lighted
		if(vec3::dot(r.m_AB, localNormal) > 0) {
			// Have same direction
			localNormal = -1.0 * localNormal;
		}

		// Return base colour
		localColour = m_baseColour;

		// Store the uv coordinates for possible later use
		m_uvPoint[0] = u;
		m_uvPoint[1] = v;

		return true;
	}

};

