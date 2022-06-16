#include "torus.hpp"
#include <cmath>
#include "Quartic/quartic.h"

namespace fRT {
	// Default constructor and destructor
	torus::torus() {}
	torus::~torus() {}

	// Function to check intersections
	bool torus::checkIntersection(
		const ray& _r,
		qbVector<double>& intPoint,
		qbVector<double>& localNormal,
		qbVector<double>& localColour
	) {
		// Convert ray to local coordinates
		ray bckRay = m_transformMatrix.apply(_r, fRT::BCKTFRM);

		vec3 P = bckRay.m_point1; // Start point of ray
		vec3 V = bckRay.m_AB; // Direction of ray
		V.Normalize();

		// Radii of torus
		double R = 0.8;
		double r = 0.5;

		// Compute times of intersection

		double p = pow(P.norm(), 2) + R * R - r * r;
		double v = 1.0; // Magnitude of V, always = 1
		double d = 2.0 * (P[0] * V[0] + P[1] * V[1] + P[2] * V[2]);
		double a = 4.0 * R * R;
		double b = a * (pow(V[0], 2) + pow(V[1], 2));
		double c = a * 2.0 * (P[0] * V[0] + P[1] * V[1]);
		double e = a * (pow(P[0], 2) + pow(P[1], 2));

		// Constants of quartic equation
		double A = 1.0;
		double B = 2 * d;
		double C = pow(d, 2) + 2 * p - b;
		double D = 2.0 * d * p - c;
		double E = pow(p, 2) - e;

		int nRoots = 0;
		std::vector<double> times(4, 1e12);

		// Solve to get times at intersection
		nRoots = solve_quartic(times, B / A, C / A, D / A, E / A);

		if(nRoots > 0) {
			double minTime = 1e12;
			for(double x : times) {
				if(x > 0.0 and x < minTime) {
					minTime = x;
				}
			}
			if(minTime > 1e9) {
				return false;
			}

			// Find point of intersection
			vec3 poi = P + minTime * V;

			// Centre at distance R from origin from which normal will be taken
			double ratio = 1.0 - (R / (sqrt(pow(poi[0], 2) + pow(poi[1], 2))));

			vec3 normal{ {ratio * poi[0], ratio * poi[1], poi[2]} };
			normal.Normalize();

			vec3 localOrigin({ 0.0, 0.0, 0.0 });

			// Convert local poi to global coordinates
			intPoint = m_transformMatrix.apply(poi, fRT::FWDTFRM);

			// Convert normal to global coordinates
			vec3 globalOrigin = m_transformMatrix.apply(localOrigin, fRT::FWDTFRM);
			localNormal = m_transformMatrix.apply(normal, fRT::FWDTFRM) - globalOrigin;
			localNormal.Normalize();

			if(vec3::dot(_r.m_AB, localNormal) > 0) {
				localNormal = -1.0 * localNormal;
			}

			// Return base colour
			localColour = m_baseColour;

			return true;
		}
		else {
			return false;
		}
	}
};