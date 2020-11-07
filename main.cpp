#include <vld.h>
#include <DSound.h>

#include "DDWAudio.h"
#include "DDWMasterParser.h"

int main()
{
	std::cout << std::endl;

	//Get the audio devices
	DDWAudio::GetInstance().Initialize();

	std::cout << std::endl;

	//Print to see available devices
	DDWAudio::GetInstance().PrintAudioDevices();

	std::cout << std::endl;

	//Select a device
	DDWAudio::GetInstance().SelectAudioDevice(1);

	std::cout << std::endl;
	
	//Print the current device
	DDWAudio::GetInstance().PrintCurrentAudioDevice();

	std::cout << std::endl;

	LPDIRECTSOUNDBUFFER secondaryBuffer = nullptr;

	DDWMasterParser::GetInstance().LoadAudioFile("Trunk.wav", &secondaryBuffer);

	//Play the buffer
	secondaryBuffer->SetCurrentPosition(0);
	secondaryBuffer->SetVolume(DSBVOLUME_MAX);
	secondaryBuffer->Play(0, 0, 0);

	while (true)
	{
	}

	secondaryBuffer->Release();

	std::cout << std::endl;

	return 1;
}
