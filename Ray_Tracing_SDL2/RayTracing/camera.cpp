#include "camera.hpp"
#include "ray.hpp"
#include <math.h>

namespace fRT {
	// Default constructor
	camera::camera() {
		// Assuming screen is along +ve Y axis, and UP is along +ve Z axis
		m_cameraPosition = qbVector<double>({ 0.0, -10.0, 0.0 });
		m_cameraLookAt = qbVector<double>({ 0.0, 0.0, 0.0 });
		m_cameraUp = qbVector<double>({ 0.0, 0.0, 1.0 });
		m_cameraFocalLength = 1.0;
		m_cameraHorzSize = 1.0;
		m_cameraAspectRatio = 1.0;
	}

	camera::camera(const qbVector<double>& newPos,
		const qbVector<double>& newLookAt,
		const qbVector<double>& newUp,
		double newFocalLength,
		double newHorzSize,
		double newAspectRatio
	) {
		setPosition(newPos);
		setLookAt(newLookAt);
		setUp(newUp);
		setFocalLength(newFocalLength);
		setHorzSize(newHorzSize);
		setAspectRatio(newAspectRatio);
	}

	void camera::setPosition(const qbVector<double>& newPos) {
		m_cameraPosition = newPos;
	}

	void camera::setLookAt(const qbVector<double>& newLookAt) {
		m_cameraLookAt = newLookAt;
	}

	void camera::setUp(const qbVector<double>& newUp) {
		m_cameraUp = newUp;
	}

	void camera::setFocalLength(double newFocalLength) {
		m_cameraFocalLength = newFocalLength;
	}

	void camera::setHorzSize(double newHorzSize) {
		m_cameraHorzSize = newHorzSize;
	}

	void camera::setAspectRatio(double newAspectRatio) {
		m_cameraAspectRatio = newAspectRatio;
	}

	// Method to return camera position
	qbVector<double> camera::getPosition() const {
		return m_cameraPosition;
	}

	// Method to return LookAt
	qbVector<double> camera::getLookAt() const {
		return m_cameraLookAt;
	}

	// Method to return CameraUp
	qbVector<double> camera::getUp() const {
		return m_cameraUp;
	}

	// Method to return focal length
	double camera::getFocalLength() const {
		return m_cameraFocalLength;
	}

	// Method to return camera horizontal screen size
	double camera::getHorzSize() const {
		return m_cameraHorzSize;
	}

	// Method to return aspect ratio
	double camera::getAspectRatio() const {
		return m_cameraAspectRatio;
	}

	// Method to return U vector
	qbVector<double> camera::getU() const {
		return m_projectionScreenU;
	}

	// Method to return V vector
	qbVector<double> camera::getV() const {
		return m_projectionScreenV;
	}

	// Method to return screen centre
	qbVector<double> camera::getScreenCentre() const {
		return m_projectionScreenCentre;
	}

	// Function to compute camera geometry
	void camera::UpdateCameraGeometry() {
	// First, compute the vector from the camera position to the LookAt position.
		m_alignmentVector = m_cameraLookAt - m_cameraPosition;
		m_alignmentVector.Normalize();

		// Second, compute the U and V vectors.
		m_projectionScreenU = qbVector<double>::cross(m_alignmentVector, m_cameraUp);
		m_projectionScreenU.Normalize();
		m_projectionScreenV = qbVector<double>::cross(m_projectionScreenU, m_alignmentVector);
		m_projectionScreenV.Normalize();

		// Thirdly, compute the positon of the centre point of the screen.
		m_projectionScreenCentre = m_cameraPosition + (m_cameraFocalLength * m_alignmentVector);

		// Modify the U and V vectors to match the size and aspect ratio.
		m_projectionScreenU = m_projectionScreenU * m_cameraHorzSize;
		m_projectionScreenV = m_projectionScreenV * (m_cameraHorzSize / m_cameraAspectRatio);

	}

	// Method to generate ray from centre of camera to point on screen
	// X and Y are between -1 and 1 representing a point in the UV coordinate system
	// Changes camera ray to be the generated ray
	bool camera::generateRay(float proScreenX, float proScreenY, ray& cameraRay) {
		// Compute location of screen point from UV system to world system

		qbVector<double> screenWorldPoint = m_projectionScreenCentre + 
			(m_projectionScreenU * proScreenX) + (m_projectionScreenV * proScreenY);

		// Use this point with camera position to calculate ray
		cameraRay.m_point1 = m_cameraPosition;
		cameraRay.m_point2 = screenWorldPoint;
		cameraRay.m_AB = screenWorldPoint - m_cameraPosition;
		
		return true;
	}
};
