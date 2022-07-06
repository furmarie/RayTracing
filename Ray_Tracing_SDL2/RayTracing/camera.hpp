#ifndef CAMERA_H
#define CAMERA_H

#include "qbLinAlg/qbVector.h"
#include "ray.hpp"

namespace fRT {
	class camera {
	public:
		// Default constructor
		camera();

		camera(
			const qbVector<double>& newPos,
			const qbVector<double>& newLookAt,
			const qbVector<double>& newUp,
			double newFocalLength,
			double newHorzSize,
			double newAspectRatio
		);

		// Functions for setting camera parameters
		void setPosition(const qbVector<double>& newPos);
		void setLookAt(const qbVector<double>& newLookAt);
		void setUp(const qbVector<double>& newUp);
		void setFocalLength(double newFocalLength);
		void setHorzSize(double newHorzSize);
		void setAspectRatio(double newAspectRatio);

		// Functions to return camera parameters
		qbVector<double> getPosition() const;
		qbVector<double> getLookAt() const;
		qbVector<double> getUp() const;
		qbVector<double> getU() const; // Vectors for pixel on screen
		qbVector<double> getV() const;
		qbVector<double> getScreenCentre() const;
		double getFocalLength() const;
		double getHorzSize() const;
		double getVertSize() const;
		double getAspectRatio() const;

		// Function to generate a ray
		bool generateRay(float proScreenX, float proScreenY, ray& cameraRay);

		// Function to update the camera geometry
		void UpdateCameraGeometry();

	private:
		qbVector<double> m_cameraPosition{ 3 }; // Origin of camera
		qbVector<double> m_cameraLookAt{ 3 }; // Point camera looks at
		qbVector<double> m_cameraUp{ 3 }; // Direction defining UP for the camera
		double			 m_cameraFocalLength;
		double			 m_cameraHorzSize;  // Width
		double			 m_cameraAspectRatio; // Width / Height
		double			 m_cameraVertSize;  // Height

		qbVector<double> m_alignmentVector{ 3 };
		qbVector<double> m_projectionScreenU{ 3 };
		qbVector<double> m_projectionScreenV{ 3 };
		qbVector<double> m_projectionScreenCentre{ 3 };
	};
};

#endif