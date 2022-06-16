#ifndef CHECKER_H
#define CHECKER_H

#include "texturebase.hpp"

namespace fRT {
	namespace texture {
		class checker : public textureBase {
		public:
			// Constructor and destructor
			checker();

			virtual ~checker() override;

			// Function to return the colour
			virtual qbVector<double> getColour(qbVector<double>& uvPoint) override;

			// Function to set colours
			void setColour(const qbVector<double>& inputColour1, const qbVector<double>& inputColour2);

		private:
			qbVector<double> m_colour1{ 4 }, m_colour2{ 4 };
		};
	}
}

#endif

