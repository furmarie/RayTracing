#include "sphere.hpp"
#include <cmath>

namespace fRT {
	// Default constructor
	sphere::sphere() {

	}

	// Destructor
	sphere::~sphere() {

	}

	// Function to check for intersection
	bool sphere::checkIntersection(const ray& r,
		qbVector<double>& intPoint,
		qbVector<double>& localNormal,
		qbVector<double>& localColour
	) const {
		// Copy ray and apply backward transform on ray to transform it to
		// the sphere's local coordinate system from world coordinate system
		ray bckRay = m_transformMatrix.apply(r, BCKTFRM);

		// Compute values of a, b, c
		qbVector<double> v_hat = bckRay.m_AB;
		v_hat.Normalize();
		/* 
		 a is equal to magnitude squared of direction of cat ray.
		 Since, direction is unit vector, a is always 1
		*/
		// a = 1.0
		double a = 1.0;

		// Calculate b
		double b = 2.0 * qbVector<double>::dot(bckRay.m_point1, v_hat);

		// Calculate c
		double c = qbVector<double>::dot(bckRay.m_point1, bckRay.m_point1) - 1.0;

		// Test whether there is an intersection
		double discriminant = (b * b) - 4.0 * a * c;

		vec3 poi; // Point of intersection in sphere coordinate system
		if(discriminant > 0.0) {
			double sqrt_disc = sqrtf(discriminant);
			double t1 = (-b - sqrt_disc) / (2 * a);
			double t2 = (-b + sqrt_disc) / (2 * a);

			/*
			  If either t1 or t2 are negative, one part of object is 
			  before the camera therefore we can ignore it
			*/
			if(t1 < 0.0 or t2 < 0.0) {
				return false;
			}
			else {
				// Determine point of intersection closest to camera
				if(t1 < t2) {
					poi = bckRay.m_point1 + t1 * v_hat;
				}
				else {
					poi = bckRay.m_point1 + t2 * v_hat;
				}

				// Transform point of intersection back to world coordinates
				intPoint = m_transformMatrix.apply(poi, FWDTFRM);

				// Compute localNormal
				vec3 objOrigin = vec3({ 0.0, 0.0, 0.0 });
				vec3 newObjOrigin = m_transformMatrix.apply(objOrigin, FWDTFRM);

				localNormal = intPoint - newObjOrigin;
				localNormal.Normalize();

				// Return base colour
				localColour = m_baseColour;

				return true;
			}
		}
		else {
			return false;
		}
	}
};
