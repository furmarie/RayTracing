#include "cylinder.hpp"
#include <cmath>
#include <array>
#include <algorithm>

namespace fRT {
	// Default constructor
	cylinder::cylinder() {}

	cylinder::~cylinder() {}

	//Function to check for intersections
	//bool cylinder::checkIntersection(
	//	const ray& r,
	//	vec3& intPoint,
	//	vec3& localNormal,
	//	vec3& localColour
	//) const {
	//	// Convert ray to local coordinate
	//	ray bckRay = m_transformMatrix.apply(r, fRT::BCKTFRM);

	//	// Copy the m_AB vector from bckRay and normalize
	//	// v - direction of vector
	//	vec3 v = bckRay.m_AB;
	//	v.Normalize();

	//	// Get startpoint of vector
	//	vec3 p = bckRay.m_point1;

	//	// Solving for equation of cylinder
	//	double a = v[0] * v[0] + v[1] * v[1];
	//	double b = 2 * (p[0] * v[0] + p[1] * v[1]);
	//	double c = p[0] * p[0] + p[1] * p[1] - 1.0 * 1.0; // Radius of cylinder is 1

	//	// Compute b^2 - 4ac
	//	double disc = b * b - 4 * a * c;

	//	// Check for intersection
	//	// First check with cylinder itself

	//	// Check at all 4 possible times
	//	std::vector<double> times(4, 1e9);

	//	if(disc > 0.0) {
	//		double sqrtDisc = sqrt(disc);


	//		times[0] = (-b - sqrtDisc) / (2.0 * a);
	//		times[1] = (-b + sqrtDisc) / (2.0 * a);
	//		if(times[0] <= 0.0 or (fabs(p[2] + times[0] * v[2]) >= 1.0)) {
	//			times[0] = 1e9;
	//		}
	//		if(times[1] <= 0.0 or (fabs(p[2] + times[1] * v[2]) >= 1.0)) {
	//			times[1] = 1e9;
	//		}

	//	}

	//	// Add times for intersection with top and bottom faces
	//	if(not isClose(v[2], 0.0)) {
	//		times[2] = (bckRay.m_point1[2] - 1.0) / -v[2];
	//		times[3] = (bckRay.m_point1[2] + 1.0) / -v[2];
	//	}

	//	for(int i = 0; i < 4; i++) {
	//		if(times[i] < 1e7) {
	//			break;
	//		}
	//		if(i == 3) return false;
	//	}

	//	int minIndex = 0;
	//	double minValue = 10e6;
	//	for(int i = 0; i < 4; ++i) {
	//		if(times[i] < minValue) {
	//			minValue = times[i];
	//			minIndex = i;
	//		}
	//	}
	//	if(times[minIndex] > 1e8 or times[minIndex] < 0) {
	//		return false;
	//	}
	//	//std::cerr << times[min_idx] << std::endl;
	//	vec3 validPOI = p + times[minIndex] * v;

	//	if(minIndex < 2) {
	//// Transform the intersection point back into world coordinates.
	//		intPoint = m_transformMatrix.apply(validPOI, fRT::FWDTFRM);

	//		// Compute the local normal.
	//		vec3 orgNormal{ 3 };
	//		vec3 newNormal{ 3 };
	//		vec3 localOrigin{ std::vector<double> {0.0, 0.0, 0.0} };
	//		vec3 globalOrigin = m_transformMatrix.apply(localOrigin, fRT::FWDTFRM);
	//		orgNormal.SetElement(0, validPOI.GetElement(0));
	//		orgNormal.SetElement(1, validPOI.GetElement(1));
	//		orgNormal.SetElement(2, 0.0);
	//		newNormal = m_transformMatrix.apply(orgNormal, fRT::FWDTFRM) - globalOrigin;
	//		newNormal.Normalize();
	//		localNormal = newNormal;

	//		// Return the base color.
	//		localColour = m_baseColour;

	//		return true;
	//	}
	//	else {
	//		// Otherwise check the end caps.
	//		if(!isClose(v.GetElement(2), 0.0)) {
	//			// Check if we are inside the disk.
	//			if(sqrtf(validPOI[0] * validPOI[0] + validPOI[1] * validPOI[1]) < 1.0) {
	//				// Transform the intersection point back into world coordinates.
	//				intPoint = m_transformMatrix.apply(validPOI, fRT::FWDTFRM);

	//				// Compute the local normal.
	//				vec3 localOrigin{ std::vector<double> {0.0, 0.0, 0.0} };
	//				vec3 normalVector{ std::vector<double> {0.0, 0.0, 0.0 + validPOI.GetElement(2)} };
	//				vec3 globalOrigin = m_transformMatrix.apply(localOrigin, fRT::FWDTFRM);
	//				localNormal = m_transformMatrix.apply(normalVector, fRT::FWDTFRM) - globalOrigin;
	//				localNormal.Normalize();

	//				// Return the base color.
	//				localColour = m_baseColour;

	//				return true;
	//			}
	//			else {
	//				return false;
	//			}
	//		}
	//		else {
	//			return false;
	//		}
	//	}

	//}
	// 
	// 
	// 
	// 
	bool cylinder::checkIntersection(
		const ray& r,
		vec3& intPoint,
		vec3& localNormal,
		vec3& localColour
	) {
	// Copy the ray and apply the backwards transform.
		ray bckRay = m_transformMatrix.apply(r, fRT::BCKTFRM);

		// Copy the m_lab vector from bckRay and normalize it.
		vec3 v = bckRay.m_AB;
		v.Normalize();

		// Get the start point of the line.
		vec3 p = bckRay.m_point1;

		// Compute a, b and c.
		double a = v[0] * v[0] + v[1] * v[1];
		double b = 2 * (p[0] * v[0] + p[1] * v[1]);
		double c = p[0] * p[0] + p[1] * p[1] - 1.0 * 1.0; // Radius of cylinder is 1

		// Compute b^2 - 4ac.
		double discriminant = b * b - 4 * a * c;

		// Test for intersections.
		std::vector<double> times(4, 1e9);

		// First with the cylinder itself.
		if(discriminant > 0.0) {
			double sqrtDisc = sqrt(discriminant);
			// There was an intersection.
			// Compute the values for t.
			times[0] = (-b + sqrtDisc) / (2 * a);
			times[1] = (-b - sqrtDisc) / (2 * a);

		}

		// And test the end caps.
		if(not isClose(v[2], 0.0)) {
			// Compute the values of t.
			times[2] = (p[2] - 1.0) / -v[2];
			times[3] = (p[2] + 1.0) / -v[2];
		}

		for(int i = 0; i < 4; i++) {
			if(times[i] < 0.0) {
				times[i] = 1e9;
			}
			if(i < 2) {  // Ray hits cylinder, check the z value only
				double poi_z = p[2] + times[i] * v[2];
				if(fabs(poi_z) >= 1.0) {
					times[i] = 1e9;
				}
			}
			else { // Ray hits end caps
				vec3 poi = p + times[i] * v;
				if(((poi[0] * poi[0] + poi[1] * poi[1]) >= 1.0)) { // Testing to see if x, y are inside disk
					times[i] = 1e9;
				}
			}
		}

		// Check for the smallest valid value of t.
		// If no valid intersections found, then we can stop.
		int minIndex = std::min_element(times.begin(), times.end()) - times.begin();
		double minValue = times[minIndex];
		if(minValue >= 1e7) {
			return false;
		}

		vec3 validPOI = p + times[minIndex] * v;

		if(minIndex >= 2) {
			if(not isClose(v[2], 0.0)) {
				// Transform the intersection point back into world coordinates.
				intPoint = m_transformMatrix.apply(validPOI, fRT::FWDTFRM);

				// Compute the local normal.
				vec3 localOrigin{ {0.0, 0.0, 0.0} };
				vec3 normalVector{ {0.0, 0.0, 0.0 + validPOI[2]} };
				vec3 globalOrigin = m_transformMatrix.apply(localOrigin, fRT::FWDTFRM);
				localNormal = m_transformMatrix.apply(normalVector, fRT::FWDTFRM) - globalOrigin;
				localNormal.Normalize();

				// Check if ray direction and normal have same direction
				if(vec3::dot(r.m_AB, localNormal) > 0) {
					// Have same direction
					localNormal = -1.0 * localNormal;
				}
				// Return the base color.
				localColour = m_baseColour;

				// Compute and store uv coordinates for later
				double x = validPOI[0];
				double y = validPOI[1];
				double z = validPOI[2];

				m_uvPoint[0] = x;
				m_uvPoint[1] = y;

				return true;
			}
			else {
				return false;
			}
		}
		else {
			// Transform the intersection point back into world coordinates.
			intPoint = m_transformMatrix.apply(validPOI, fRT::FWDTFRM);

			// Compute the local normal.
			vec3 orgNormal{ {0, 0, 0} };
			vec3 newNormal{ 3 };
			vec3 localOrigin{ {0.0, 0.0, 0.0} };
			vec3 globalOrigin = m_transformMatrix.apply(localOrigin, fRT::FWDTFRM);
			orgNormal[0] = validPOI[0];
			orgNormal[1] = validPOI[1];

			newNormal = m_transformMatrix.apply(orgNormal, fRT::FWDTFRM) - globalOrigin;
			newNormal.Normalize();
			localNormal = newNormal;

			// Check if direction of ray and localNormal are same
			if(vec3::dot(r.m_AB, localNormal) > 0) {
				// Have same direction
				localNormal = -1.0 * localNormal;
			}

			// Return the base color.
			localColour = m_baseColour;

			// Calculate and store uv coordinates for later
			double x = validPOI[0];
			double y = validPOI[1];
			double z = validPOI[2];

			double u = atan2(y, x) / PI;
			double v = z;

			m_uvPoint[0] = u;
			m_uvPoint[1] = v;

			return true;
		}

		return false;
	}
};
