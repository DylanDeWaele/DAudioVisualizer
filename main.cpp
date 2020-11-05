#include <vld.h>
#include <DSound.h>

#include "DDWAudio.h"
#include "DDWMasterParser.h"

int main()
{
	DDWAudio* pAudioEngine = new DDWAudio{};

	std::cout << std::endl;

	//Get the audio devices
	pAudioEngine->Initialize();

	std::cout << std::endl;

	//Print to see available devices
	pAudioEngine->PrintAudioDevices();

	std::cout << std::endl;

	//Select a device
	pAudioEngine->SelectAudioDevice(1);

	//Print the current device
	pAudioEngine->PrintCurrentAudioDevice();

	//Play the primary sound buffer
	pAudioEngine->GetPrimarySoundBuffer()->Play(0, 0, DSBPLAY_LOOPING);

	std::cout << std::endl;

	DDWMasterParser::GetInstance().LoadAudioFile("Trunk.wav", nullptr);

	std::cout << std::endl;

	delete pAudioEngine;

	return 1;
}
