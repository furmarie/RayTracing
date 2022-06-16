#ifndef FLAT_H
#define FLAT_H

#include "texturebase.hpp"


namespace fRT {
	namespace texture {
		class flat : textureBase{
			// Constructor and destructor
		public:
			flat();
			virtual ~flat() override;

			// Function to return colour at uv
			virtual qbVector<double> getColour(qbVector<double>& uvPoint) override;

			// Function to set colour
			void setColour(const qbVector<double>& inputColour);

		private:
			qbVector<double> m_colour{4};

		};
	};
};

#endif
