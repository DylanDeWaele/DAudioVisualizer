#include <vld.h>
#include <SDL.h>
#include <iostream>

#include "DDWAudio.h"
#include "DDWGraphics.h"
#include "DDWSound.h"
#include "DDWChannel.h"

#undef main

int main()
{
	//Initialize Audio engine
	DDWAudio::GetInstance().Initialize();
	
	//Initialize Graphics engine
	DDWGraphics::GetInstance().Initialize();

	//Play sound
	DDWSound* pSound = new DDWSound{ "Trunk.wav" };
	pSound->Play();	

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
			DDWAudio::GetInstance().Update();

			//Draw
			SDL_UpdateWindowSurface(pWindow);
		}
	}

	delete pSound;

	return 1;
}
