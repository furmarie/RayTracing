#include "gtfm.hpp"

namespace fRT {
	// Constructor / destructor
	GTform::GTform() {
		// Set fwd and bck transforms to identity matrices
		m_fwdtfm.SetToIdentity();
		m_bcktfm.SetToIdentity();
	}

	GTform::~GTform() {}

	// Construct from a pair of matrices
	GTform::GTform(const matrix& fwd, const matrix& bck) {
		// Verify input matrices are 4 x 4
		if((fwd.GetNumRows() != 4 or fwd.GetNumCols() != 4) or
			(bck.GetNumRows() != 4 or bck.GetNumCols() != 4)
			) {
			throw std::invalid_argument("Cannot construct GTform inputs are not all 4x4.");
		}
		m_fwdtfm = fwd;
		m_bcktfm = bck;
	}

	// Construct from three vector
	GTform::GTform(
		const vec3& translation,
		const vec3& rotation,
		const vec3& scale
	) {
		setTransform(translation, rotation, scale);
	}

	void GTform::setTransform(
		const vec3& translation,
		const vec3& rotation,
		const vec3& scale
	) {
		// Make sure that no component in scale is zero

		 // Define matrix for each component of transform
		matrix translationMatrix{ 4, 4 };
		matrix rotationMatrixX{ 4, 4 };
		matrix rotationMatrixY{ 4, 4 };
		matrix rotationMatrixZ{ 4, 4 };
		matrix scaleMatrix{ 4, 4 };

		// Set to identity;
		translationMatrix.SetToIdentity();
		rotationMatrixX.SetToIdentity();
		rotationMatrixY.SetToIdentity();
		rotationMatrixZ.SetToIdentity();
		scaleMatrix.SetToIdentity();

		// Populate with appropriate values
		// First, translation
		translationMatrix[{0, 3}] = translation[0];
		translationMatrix[{1, 3}] = translation[1];
		translationMatrix[{2, 3}] = translation[2];

		// Second, rotation matrices
		double theta_x = rotation[0];
		double theta_y = rotation[1];
		double theta_z = rotation[2];

		rotationMatrixX[{1, 1}] = cos(theta_x);
		rotationMatrixX[{1, 2}] = -sin(theta_x);
		rotationMatrixX[{2, 1}] = sin(theta_x);
		rotationMatrixX[{2, 2}] = cos(theta_x);

		rotationMatrixY[{0, 0}] = cos(theta_y);
		rotationMatrixY[{0, 2}] = sin(theta_y);
		rotationMatrixY[{2, 0}] = -sin(theta_y);
		rotationMatrixY[{2, 2}] = cos(theta_y);

		rotationMatrixZ[{0, 0}] = cos(theta_z);
		rotationMatrixZ[{0, 1}] = -sin(theta_z);
		rotationMatrixZ[{1, 0}] = sin(theta_z);
		rotationMatrixZ[{1, 1}] = cos(theta_z);


		// Third, scale matrix
		scaleMatrix[{0, 0}] = scale[0];
		scaleMatrix[{1, 1}] = scale[1];
		scaleMatrix[{2, 2}] = scale[2];

		// Combine to get forward transform matrix
		m_fwdtfm = translationMatrix *
			rotationMatrixX *
			rotationMatrixY *
			rotationMatrixZ *
			scaleMatrix;

// Compute backward transform
		m_bcktfm = m_fwdtfm;
		m_bcktfm.Inverse();
	}

	// Functions to return transform matrices
	matrix GTform::getForward() {
		return m_fwdtfm;
	}

	matrix GTform::getBackward() {
		return m_bcktfm;
	}

	// Function to apply transform
	ray GTform::apply(const ray& inputRay, bool dirFlag) const {
		// Create output object
		ray res;

		// Apply transforms to point1 and point2 of ray
		res.m_point1 = this->apply(inputRay.m_point1, dirFlag);
		res.m_point2 = this->apply(inputRay.m_point2, dirFlag);
		res.m_AB = res.m_point2 - res.m_point1;

		return res;
	}

	vec3 GTform::apply(const vec3& inputVec, bool dirFlag) const {
		 // Convert inputVec to 4 element vec
		std::vector<double> temp(inputVec.data());
		temp.push_back(1.0);
		vec3 tempVec{ temp };

		// Create result vector
		vec3 res;
		//printMatrix(dirFlag);
		//printVector(tempVec);
		res = (dirFlag ? m_fwdtfm : m_bcktfm) * tempVec;

		// Make output vector as a 3-element vector
		res = vec3({ res[0], res[1], res[2] });

		return res;
	}

	// Overload operators

	fRT::GTform operator* (const fRT::GTform& lhs, const fRT::GTform& rhs) {
		// Form product of the two forward transforms
		matrix fwdRes = lhs.m_fwdtfm * rhs.m_bcktfm;
		matrix bckRes = fwdRes;
		bckRes.Inverse();

		// Form result
		fRT::GTform res(fwdRes, bckRes);
		return res;
	}

	// Overload assignment operator
	GTform GTform::operator= (const GTform& rhs) {
		// Make sure not assigning to ourself
		if(this != &rhs) {
			m_fwdtfm = rhs.m_fwdtfm;
			m_bcktfm = rhs.m_bcktfm;
		}
		return *this;
	}

	// Function to print transform matrix
	void GTform::printMatrix(bool dirFlag) const {
		print(dirFlag ? m_fwdtfm : m_bcktfm);
	}

	void GTform::print(const matrix& mat) const {
		int nRows = mat.GetNumRows();
		int nCols = mat.GetNumCols();
		for(int i = 0; i < nRows; i++) {
			for(int j = 0; j < nCols; j++) {
				std::cout << std::fixed << std::setprecision(3) << mat[std::make_pair(i, j)] << " ";
			}
			std::cout << std::endl;
		}
	}

	void GTform::printVector(vec3& vec) const {
		for(auto x : vec.data()) {
			std::cout << std::fixed << std::setprecision(3) << x << " ";
		}
		std::cout << std::endl;
	}
};