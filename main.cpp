#include <vld.h>
#include "DAudio.h"
#include "DSound.h"

int main()
{
	std::cout << std::endl;

	//Get the audio devices
	DAudio::GetInstance().Initialize();

	std::cout << std::endl;

	//Print to see available devices
	DAudio::GetInstance().PrintAudioDevices();
	
	std::cout << std::endl;

	//Select a device
	DAudio::GetInstance().SelectAudioDevice(1);

	//Print the current device
	DAudio::GetInstance().PrintCurrentAudioDevice();

	std::cout << std::endl;	
	
	return 1;
}
