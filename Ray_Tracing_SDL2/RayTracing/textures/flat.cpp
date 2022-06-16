#include "flat.hpp"

namespace fRT {
	// Constructor and destructor
	texture::flat::flat() {
		m_colour = qbVector<double>({1.0, 0.65, 0.0, 1.0});
	}

	texture::flat::~flat() {}

	// Function to return colour
	qbVector<double> texture::flat::getColour(qbVector<double>& uvPoint) {
		return m_colour;
	}

	// Function to set colour
	void texture::flat::setColour(const qbVector<double>& inputColour) {
		m_colour = inputColour;
	}
}
