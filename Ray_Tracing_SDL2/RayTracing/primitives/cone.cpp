#include "cone.hpp"
#include <cmath>
#include <algorithm>

namespace fRT {
	cone::cone() {}

	cone::~cone() {}


	// Function to check for intersection
	bool cone::checkIntersection(
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
		double a = v[0] * v[0] + v[1] * v[1] - v[2] * v[2];
		double b = 2 * (p[0] * v[0] + p[1] * v[1] - p[2] * v[2]);
		double c = p[0] * p[0] + p[1] * p[1] - p[2] * p[2];

		// Compute b^2 - 4ac.
		double discriminant = b * b - 4 * a * c;

		// Test for intersections.
		std::vector<double> times(3, 1e9);

		// First with the cone itself.
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
		}

		for(int i = 0; i < 3; i++) {
			if(times[i] < 0.0) {
				times[i] = 1e9;
			}
			if(i < 2) {  // Ray hits cone, check the z value only
				double poi_z = p[2] + times[i] * v[2];
				if(fabs(poi_z) >= 1.0 or (poi_z < 0.0)) {
					times[i] = 1e9;
				}
			}
			else { // Ray hits end caps
				vec3 poi = p + times[i] * v;
				if(((poi[0] * poi[0] + poi[1] * poi[1]) >= poi[2] * poi[2]) or (poi[2] < 0.0)) { // Testing to see if x, y are inside disk and z value is less than 0
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

		if(validPOI[2] >= 0.0) {
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

					// Compute and store uv coordinates
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
				orgNormal[2] = -sqrt(validPOI[0] * validPOI[0] + validPOI[1] * validPOI[1]);

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

				// Compute and store uv coordinates
				double x = validPOI[0];
				double y = validPOI[1];
				double z = validPOI[2];

				double u = atan2(y, x) / PI;
				double v = z * 2.0 + 1; // 0 <= z <= 1

				m_uvPoint[0] = u;
				m_uvPoint[1] = v;

				return true;
			}
		}
		return false;
	}
};