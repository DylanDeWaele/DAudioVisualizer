#include <SFML/Graphics.hpp>
#include <vld.h>
#include <DSound.h>

#include "DDWAudio.h"
#include "DDWMasterParser.h"
#include "DDWSound.h"

int main()
{
	//Create SFML window
	sf::RenderWindow window(sf::VideoMode(800, 600), "DAudioVisualizer");

	//Initialize Audio
	DDWAudio::GetInstance().Initialize();

	//Create a sound
	DDWSound sound{ "Trunk.wav" };
	sound.Play();

	//Game loop
	while (window.isOpen())
	{
		//Poll input events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Draw loop
		{
			window.clear();
			window.display();
		}
	}

	return 1;
}
