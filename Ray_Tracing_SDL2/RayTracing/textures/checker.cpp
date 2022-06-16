#include "checker.hpp"

namespace fRT {
	// Constructor and destructor
	texture::checker::checker() {
		m_colour1 = qbVector<double>({ 1.0, 1.0, 1.0, 1.0 });
		m_colour2 = qbVector<double>({ 0.5, 0.5, 0.5, 1.0 });
	}

	texture::checker::~checker() {}

	// Function to return the colour
	qbVector<double> texture::checker::getColour(qbVector<double>& uvPoint) {
		// Apply local transform to the uv coordinates
		qbVector<double> inputLoc = uvPoint;
		qbVector<double> newLoc = applyTransform(inputLoc);

		double newU = newLoc[0];
		double newV = newLoc[1];
		
		qbVector<double> localColour {4};
		int check = floor(newU) + floor(newV);

		if(check % 2 == 0) {
			localColour = m_colour1;
		}
		else {
			localColour = m_colour2;
		}
		return localColour;
	}

	// Function to set colours
	void texture::checker::setColour(const qbVector<double>& inputColour1, const qbVector<double>& inputColour2) {
		m_colour1 = inputColour1;
		m_colour2 = inputColour2;
	}
}
