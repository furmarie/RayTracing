#ifndef SPHERE_H
#define SPHERE_H

#include "objectbase.hpp"

namespace fRT {
	class sphere : public objectBase {
	public:
		/*
		 Default constructor
		 Note: This will define a unit sphere centred at the origin
		*/ 
		sphere();

		// Override the destructor
		virtual ~sphere() override;

		// Override function to test for intersection
		virtual bool checkIntersection(const ray& r,
			qbVector<double>& intPoint,
			qbVector<double>& localNormal,
			qbVector<double>& localColour
		) const override;

	private:

	};
};

#endif