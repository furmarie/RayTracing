#ifndef CYLINDER_H
#define CYLINDER_H

#include "objectbase.hpp"

namespace fRT {
	class cylinder : public objectBase {
	public:
		// Default constructor
		cylinder();

		// Override the destructor
		virtual ~cylinder() override;

		// Override the function to check for intersection
		virtual bool checkIntersection(
			const ray& r,
			qbVector<double>& intPoint,
			qbVector<double>& localNormal,
			qbVector<double>& localColour
		) override;


	};
};


#endif