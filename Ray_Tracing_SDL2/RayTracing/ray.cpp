#include "ray.hpp"

namespace fRT {
	ray::ray() :
		m_point1(qbVector<double>({ 0.0, 0.0, 0.0 })),
		m_point2(qbVector<double>({ 0.0, 0.0, 1.0 })),
		m_AB(m_point2 - m_point1) 
	{}

		ray::ray(const qbVector<double>& p1, const qbVector<double>& p2)
		: m_point1(p1), m_point2(p2), m_AB(p2 - p1) {
	}

	qbVector<double> ray::getPoint1() const {
		return m_point1;
	}

	qbVector<double> ray::getPoint2() const {
		return m_point2;
	}
};