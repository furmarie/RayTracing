#include <iostream>

#include "CApp.h"
#include "RayTracing/qbLinAlg/qbVector.h"


// Default constructur
CApp::CApp() {
	isRunning = true;
	pWindow = NULL;
	pRenderer = NULL;
}

bool CApp::OnInit() {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}

	pWindow = SDL_CreateWindow("Ray Tracing SDL",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, IMG_WIDTH, IMG_HEIGHT, SDL_WINDOW_SHOWN);

	if(pWindow != NULL) {
		pRenderer = SDL_CreateRenderer(pWindow, -1, 0);

		// Initialise fImage instance
		m_image.Initialise(IMG_WIDTH, IMG_HEIGHT, pRenderer);

		// Set background colour
		SDL_SetRenderDrawColor(pRenderer, 255, 0, 255, 255);
		SDL_RenderClear(pRenderer);

		// Render the scene
		m_scene.Render(m_image);

		// Setup a texture
		//fRT::texture::checker testTex;
		//testTex.setTransform(qbVector<double>({0.3, 0.0}), PI / 4, qbVector<double>({4, 4}));

		// Render the texture
		//for(int y = 0; y < IMG_HEIGHT; y++) {
		//	for(int x = 0; x < IMG_WIDTH; x++) {
		//		// Compute u, v
		//		double u = (double) x / ((double) IMG_WIDTH / 2.0) - 1.0;
		//		double v = (double) y / ((double) IMG_HEIGHT / 2.0) - 1.0;
		//		qbVector<double> uvPoint ({u, v});
		//		auto pixelColour = testTex.getColour(uvPoint);

		//		// Update pixel
		//		m_image.SetPixel(x, y, pixelColour[0], pixelColour[1], pixelColour[2]);
		//	}
		//}

		// Display image
		m_image.Display();

		// Show result with SDL
		SDL_RenderPresent(pRenderer);
	}
	else {
		return false;
	}
	return true;
}

int CApp::OnExecute() {
	SDL_Event event;

	// Initialise image and drawer
	if(OnInit() == false) {
		return -1;
	}
	std::cout << "RUNNING!" << std::endl;

	// Loop and keep rendering image
	while(isRunning) {
		while(SDL_PollEvent(&event) != 0) {
			OnEvent(&event);
		}
		OnLoop();
		OnRender();
	}
	OnExit();
	return 0;
}

void CApp::OnEvent(SDL_Event* event) {
	if(event->type == SDL_QUIT) {
		isRunning = false;
	}
}

void CApp::OnLoop() {

}

void CApp::OnRender() {
	// Set background colour to something
	//SDL_SetRenderDrawColor(pRenderer, 200, 0, 190, 255);
	//SDL_RenderClear(pRenderer);

	// Render scene to the image (m_image)
	//m_scene.Render(m_image);

	// Display the image
	//m_image.Display();

	// Display result
	//SDL_RenderPresent(pRenderer);
}

void CApp::OnExit() {
	// Clean up SDL2 stuff
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	pWindow = NULL;
	SDL_Quit();
	std::cerr << "QUIT SUCCESSFULLY!" << std::endl;
}

// Private functions
void CApp::printVec(const qbVector<double>& vec) {
	int nRows = vec.GetNumDims();
	for(int i = 0; i < nRows; i++) {
		std::cout << std::fixed << std::setprecision(3) << vec.GetElement(i) << " ";
	}
	std::cout << std::endl;
}