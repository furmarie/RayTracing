#ifndef CONE_H
#define CONE_H

#include "objectbase.hpp"

namespace fRT {
	class cone : public objectBase {
	public:
		// Constructor and destructor
		cone();
		virtual ~cone() override;

		// Override function to check for intersections
		virtual bool checkIntersection(
			const ray& r,
			vec3& intPoint,
			vec3& localNormal,
			vec3& localColour
		) override;

	private:

	};

};

#endif