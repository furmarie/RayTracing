#include "lightbase.hpp"

namespace fRT {
	// Constructor
	lightBase::lightBase() {}

	// Destructor
	lightBase::~lightBase() {}

	// Function to compute illumination
	bool lightBase::computeIllumination(
		const qbVector<double>& intPoint,
		const qbVector<double>& localNormal,
		const std::vector<std::shared_ptr<objectBase>>& objectList,
		const std::shared_ptr<objectBase>& currObj,
		qbVector<double>& colour,
		double& intensity
	) {
		return false;
	}
};
