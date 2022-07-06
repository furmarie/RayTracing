#ifndef FIMAGE_H
#define FIMAGE_H

#include <string>
#include <vector>
#include <SDL.h>


class fImage {
public:
	// Constructor.
	fImage();

	// Destructor.
	~fImage();

	// Initialiser function
	void Initialise(const int m_xSize, const int m_ySize, SDL_Renderer* pRenderer);

	// Function for setting colour of pixel
	void SetPixel(const int x, const int y, const double red, const double green, const double blue);

	void Display();

	// Function to save an image
	bool saveImage();

	// Functions to get dimensions of image
	int getxSize();
	int getySize();

private:
	Uint32 ConvertColour(const double red, const double green, const double blue);
	void InitTexture();
	void computeMaxValues();

private:
	// Arrays for storing image data;
	std::vector<std::vector<double>> m_rChannel;
	std::vector<std::vector<double>> m_gChannel;
	std::vector<std::vector<double>> m_bChannel;

	// Store dimensions of image
	int m_xSize, m_ySize;

	// Store the maximum values of colour
	double m_maxRed, m_maxGreen, m_maxBlue, m_overallMax;

	// SDL2 stuff
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture;
};

#endif