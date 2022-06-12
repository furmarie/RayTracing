#ifndef RAY_H
#define RAY_H

#include "qblinalg/qbVector.h"

namespace fRT {
	class ray {
	public:
		ray();
		ray(const qbVector<double>& p1, const qbVector<double>& p2);

		qbVector<double> getPoint1() const;
		qbVector<double> getPoint2() const;

	public:
		qbVector<double> m_point1{ 3 }, m_point2{ 3 };
		qbVector<double> m_AB{ 3 }; // Vector goes from A to B
	};
};

#endif