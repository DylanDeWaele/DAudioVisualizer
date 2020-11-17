#include "DDWGraphics.h"
#include <iostream>

DDWGraphics::DDWGraphics()
	: m_pWindow{ nullptr },
	m_pWindowSurface{nullptr}
{
}


DDWGraphics& DDWGraphics::GetInstance()
{
	static DDWGraphics instance;
	return instance;
}

SDL_Window* DDWGraphics::GetWindow() const
{
	return m_pWindow;
}

void DDWGraphics::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to initialize SDL" << std::endl;
		return;
	}

	m_pWindow = SDL_CreateWindow("DAudioVisualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480, 0);

	if (!m_pWindow)
	{
		std::cout << "Failed to create window" << std::endl;
		return;
	}

	m_pWindowSurface = SDL_GetWindowSurface(m_pWindow);

	if (!m_pWindowSurface)
	{
		std::cout << "Failed to create window surface" << std::endl;
		return;
	}
}
