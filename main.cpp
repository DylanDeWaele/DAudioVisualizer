#include <vld.h>
#include "DAudio.h"
#include "DSound.h"

int main()
{
	std::cout << std::endl;

	//Get the audio devices
	DAudio::GetInstance().Initialize();

	//Print to see available devices
	DAudio::GetInstance().PrintAudioDevices();
	
	std::cout << std::endl;

	//Select a device
	DAudio::GetInstance().SelectAudioDevice(1);

	//Print the current device
	DAudio::GetInstance().PrintCurrentAudioDevice();

	std::cout << std::endl;

	//Create a sound
	DSound* pSound = new DSound{};

	delete pSound;

	return 1;
}
