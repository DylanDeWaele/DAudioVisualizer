#pragma once
#include <SDL.h>

class DDWGraphics
{
public:
	DDWGraphics();
	~DDWGraphics() = default;

	void Initialize();

	SDL_Window* GetWindow() const;

	static DDWGraphics& GetInstance();

private:
	SDL_Window* m_pWindow;
	SDL_Surface* m_pWindowSurface;
};

