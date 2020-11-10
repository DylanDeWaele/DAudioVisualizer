#include "DDWAudio.h"

#pragma region CALLBACKS
#pragma endregion

DDWAudio::DDWAudio()
{
}

DDWAudio::~DDWAudio()
{

}


void DDWAudio::PrintAudioDevices() const
{
	std::cout << "Available Audio Devices: " << std::endl;
	std::cout << "------------------------ " << std::endl;


}

void DDWAudio::PrintCurrentAudioDevice() const
{
	std::cout << "Current Audio Device: " << std::endl;
	std::cout << "--------------------- " << std::endl;
}

DDWAudio& DDWAudio::GetInstance()
{
	static DDWAudio instance{};
	return instance;
}

void DDWAudio::Initialize(int index)
{
	std::cout << "Successfully created and initialized the DAudio engine!" << std::endl;
}

void DDWAudio::SelectAudioDevice(int index)
{
	//Re-initialize the system
	Initialize(index);
}