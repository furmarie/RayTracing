#ifndef LIGHTBASE_H
#define LIGHTBASE_H

#include <memory>
#include "../qbLinAlg/qbVector.h"
#include "../ray.hpp"
#include "../primitives/objectbase.hpp"

namespace fRT {
	class lightBase {
	public:
		// Constructor
		lightBase();
		virtual ~lightBase();

		// Function to compute contribution to illumination
		virtual bool computeIllumination(
			const qbVector<double>& intPoint,
			const qbVector<double>& localNormal,
			const std::vector<std::shared_ptr<objectBase>>& objectList,
			const std::shared_ptr<objectBase>& currObj,
			qbVector<double>& colour,
			double& intensity
		);

	public:
		qbVector<double> m_colour{ 3 };
		qbVector<double> m_location {3};
		double m_intensity;
	};
};

#endif
