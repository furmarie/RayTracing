#ifndef TORUS_H
#define TORUS_H

#include "objectbase.hpp"

namespace fRT {
	class torus : public objectBase {
	public:
		// Constructor and Destructor
		torus();
		virtual ~torus() override;

		// Function to check for intersection
		virtual bool checkIntersection(
			const ray& r,
			qbVector<double>& intPoint,
			qbVector<double>& localNormal,
			qbVector<double>& localColour
		) override;

	private:

	};
};

#endif