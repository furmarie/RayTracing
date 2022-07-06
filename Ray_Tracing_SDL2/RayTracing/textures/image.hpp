#ifndef IMAGE_H
#define IMAGE_H

#include "texturebase.hpp"

namespace fRT {
	namespace texture {
		class image : textureBase{
		public:
			image();
			virtual ~image() override;

			// Function to return the colour
		};
	};
};

#endif

