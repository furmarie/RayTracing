#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "lightbase.hpp"
#include "../../consts.h"

namespace fRT {
	class pointLight : public lightBase{
	public:
		// Default constructor
		pointLight();

		// Override default destructor
		virtual ~pointLight() override;
		
		virtual bool computeIllumination(
			const qbVector<double>& intPoint,
			const qbVector<double>& localNormal,
			const std::vector<std::shared_ptr<objectBase>>& objectList,
			const std::shared_ptr<objectBase>& currObj,
			qbVector<double>& colour,
			double& intensity
		) override;
	};
};

#endif

