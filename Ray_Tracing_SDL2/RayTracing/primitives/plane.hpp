#ifndef PLANE_H
#define PLANE_H

#include "objectbase.hpp"

namespace fRT {
	class plane : public fRT::objectBase {

	public:
		// Default constructor
		plane();

		// Override the destructor
		virtual ~plane() override;

		// Override the function to check for intersections
		virtual bool checkIntersection(
			const ray& r,
			qbVector<double>& intPoint,
			qbVector<double>& localNormal,
			qbVector<double>& localColour
		) const override;

	private:

	};
};

#endif