#pragma once

#ifndef CAPP_H
#define CAPP_H
#include<SDL.h>
#include "RayTracing/fImage.hpp"
#include "RayTracing/scene.hpp"
#include "RayTracing/camera.hpp"
#include "consts.h"


class CApp {
public:
	CApp();

	int OnExecute();
	bool OnInit();
	void OnEvent(SDL_Event* event);
	void OnLoop();
	void OnRender();
	void OnExit();

private:
	void printVec(const qbVector<double>& vec);

private:
	// Instance of fImage class to store image
	fImage m_image;
	
	// Instance of scene class
	fRT::Scene m_scene;

	// SDL2 code
	bool isRunning;
	SDL_Window* pWindow;
	SDL_Renderer* pRenderer;
};

#endif

