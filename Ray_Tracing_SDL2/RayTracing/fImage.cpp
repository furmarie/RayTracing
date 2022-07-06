#include "fImage.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

// Default constructor
fImage::fImage() : m_xSize(0), m_ySize(0), m_pTexture(NULL) {};

// Default destructor
fImage::~fImage() {
	if(m_pTexture != NULL) {
		SDL_DestroyTexture(m_pTexture);
	}
}

//Function to return dimensions of image
int fImage::getxSize() {
	return m_xSize;
}

int fImage::getySize() {
	return m_ySize;
}

// Initialiser function
void fImage::Initialise(const int xSize, const int ySize, SDL_Renderer* pRenderer) {
	// Store the dimensions
	m_xSize = xSize;
	m_ySize = ySize;

	// Resize the image arrays
	m_rChannel.resize(m_xSize, std::vector<double>(m_ySize, 0.0));
	m_gChannel.resize(m_xSize, std::vector<double>(m_ySize, 0.0));
	m_bChannel.resize(m_xSize, std::vector<double>(m_ySize, 0.0));


	// Store pointer to renderer
	m_pRenderer = pRenderer;

	// Initialise texture
	InitTexture();
}

// Function for setting pixels
void fImage::SetPixel(const int x, const int y, const double red, const double blue, const double green) {
	m_rChannel[x][y] = red;
	m_gChannel[x][y] = blue;
	m_bChannel[x][y] = green;
}

// Function to generate display
void fImage::Display() {
	// Compute maximum values
	computeMaxValues();

	// Allocate memory for pixel buffer
	Uint32 pixel_size = m_xSize * m_ySize;
	Uint32* tempPixels = new Uint32[pixel_size];

	// Clear pixel buffer
	memset(tempPixels, 0, pixel_size * sizeof(Uint32));

	for(int y = 0; y < m_ySize; y++) {
		for(int x = 0; x < m_xSize; x++) {
			size_t idx = y * m_xSize + x;
			tempPixels[idx] = ConvertColour(m_rChannel[x][y], m_gChannel[x][y], m_bChannel[x][y]);

		}
	}

	// Update texture with the pixel buffer
	SDL_UpdateTexture(m_pTexture, NULL, tempPixels, m_xSize * sizeof(Uint32));

	// Delete pixel buffer
	delete[] tempPixels;

	// Copy the texture to the renderer
	SDL_Rect srcRect, bounds;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = m_xSize;
	srcRect.h = m_ySize;
	bounds = srcRect;
	SDL_RenderCopy(m_pRenderer, m_pTexture, &srcRect, &bounds);
}

// Function to initialise texture
void fImage::InitTexture() {
	// Initialise texture
	Uint32 r_mask, g_mask, b_mask, a_mask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	r_mask = 0xff000000;
	g_mask = 0x00ff0000;
	b_mask = 0x0000ff00;
	a_mask = 0x000000ff;
#else
	r_mask = 0x000000ff;
	g_mask = 0x0000ff00;
	b_mask = 0x00ff0000;
	a_mask = 0xff000000;
#endif

	// Delete previously created texture
	if(m_pTexture != NULL) {
		SDL_DestroyTexture(m_pTexture);
	}

	// Create texture that will store the image
	SDL_Surface* temp_surface = SDL_CreateRGBSurface(0, m_xSize, m_ySize, 32, r_mask, g_mask, b_mask, a_mask);
	m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, temp_surface);
	SDL_FreeSurface(temp_surface);
}

double clamp(double x, double mn, double mx) {
	if(x < mn) x = mn;
	if(x > mx) x = mx;
	return x;
}

// Function to convert colour to Uint32
Uint32 fImage::ConvertColour(const double red, const double green, const double blue) {
	// Convert colours to unsigned chars
	//unsigned char ir = static_cast<unsigned char>((red / m_overallMax) * 255.0);
	//unsigned char ig = static_cast<unsigned char>((green / m_overallMax) * 255.0);
	//unsigned char ib = static_cast<unsigned char>((blue / m_overallMax) * 255.0);
	unsigned char ir = static_cast<unsigned char>(clamp(red, 0.0, 0.999) * 256.0);
	unsigned char ig = static_cast<unsigned char>(clamp(green, 0.0, 0.999) * 256.0);
	unsigned char ib = static_cast<unsigned char>(clamp(blue, 0.0, 0.999) * 256.0);
	unsigned char ia = 255;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Uint32 pixel_colour = (ib << 24) + (ig << 16) + (ir << 8) + ia;
#else
	Uint32 pixel_colour = (ia << 24) + (ir << 16) + (ig << 8) + ib;
#endif

	return pixel_colour;
}


// Function to compute maximum values
void fImage::computeMaxValues() {
	m_maxRed = 0;
	m_maxGreen = 0;
	m_maxBlue = 0;
	m_overallMax = 0;

	auto ckmax = [](double& a, double b) {
		if(b > a) {
			a = b;
		}
	};

	for(int x = 0; x < m_xSize; x++) {
		for(int y = 0; y < m_ySize; y++) {
			double red = m_rChannel[x][y];
			double green = m_gChannel[x][y];
			double blue = m_bChannel[x][y];

			ckmax(m_maxRed, red);
			ckmax(m_maxGreen, green);
			ckmax(m_maxBlue, blue);

		}
	}
	ckmax(m_overallMax, m_maxRed);
	ckmax(m_overallMax, m_maxGreen);
	ckmax(m_overallMax, m_maxBlue);
}

bool fImage::saveImage() {
		// Initialise texture
	Uint32 r_mask, g_mask, b_mask, a_mask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	r_mask = 0xff000000;
	g_mask = 0x00ff0000;
	b_mask = 0x0000ff00;
	a_mask = 0x000000ff;
#else
	r_mask = 0x000000ff;
	g_mask = 0x0000ff00;
	b_mask = 0x00ff0000;
	a_mask = 0xff000000;
#endif

	auto t = std::time(nullptr);
	struct tm now;
	localtime_s(&now, &t);
	std::stringstream ss;
	ss << std::put_time(&now, "%d-%m-%Y_%H-%M-%S");
	std::string file_name_ = "screenshots/" + ss.str() + ".bmp";
	const char* file_name = file_name_.c_str();
	SDL_Texture* target = SDL_GetRenderTarget(m_pRenderer);
	SDL_SetRenderTarget(m_pRenderer, m_pTexture);
	int width, height;
	SDL_QueryTexture(m_pTexture, NULL, NULL, &width, &height);
	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, r_mask, g_mask, b_mask, a_mask);
	SDL_RenderReadPixels(m_pRenderer, NULL, surface->format->format, surface->pixels, surface->pitch);
	//IMG_SavePNG(surface, file_name);
	if(SDL_SaveBMP(surface, file_name) != 0) {
		std::cerr << "Unable to save image!" << std::endl;
		return false;
	}
	SDL_FreeSurface(surface);
	SDL_SetRenderTarget(m_pRenderer, target);
	return true;
}

