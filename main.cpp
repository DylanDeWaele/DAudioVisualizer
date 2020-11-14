#include <vld.h>
#include <SDL.h>
#include <iostream>

#include "DDWAudio.h"
#include "DDWGraphics.h"
#include "DDWSound.h"

#undef main

int main()
{
	//Initialize Audio engine
	DDWAudio::GetInstance().Initialize();
	DDWAudio::GetInstance().PrintAudioDevices();

	//Initialize Graphics engine
	DDWGraphics::GetInstance().Initialize();

	//Play sound
	DDWSound satan{ "Trunk.wav" };
	satan.Play();

	//Game loop
	bool quit = false;
	SDL_Window* pWindow = DDWGraphics::GetInstance().GetWindow();

	while (!quit)
	{
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent) > 0)
		{
			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			}

			//Update

			//Draw
			SDL_UpdateWindowSurface(pWindow);
		}
	}

	return 1;
}
