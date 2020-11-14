#include "DDWAudio.h"
#include <iostream>

DDWAudio::DDWAudio()
	: m_AmountOfAudioDevices{},
	m_pCurrentSoundBuffer{nullptr},
	m_CurrentSoundLength{}
{
}

DDWAudio::~DDWAudio()
{
	SDL_CloseAudio();
}

void DDWAudio::PrintAudioDevices() const
{
	std::cout << "Available Audio Devices: " << std::endl;
	std::cout << "------------------------ " << std::endl;

	for (int i = 0; i < m_AmountOfAudioDevices; ++i) 
	{
		std::cout << i << ": " << SDL_GetAudioDeviceName(i, 0) << std::endl;
	}
}

void DDWAudio::PrintCurrentAudioDevice() const
{
	std::cout << "Current Audio Device: " << std::endl;
	std::cout << "--------------------- " << std::endl;

	std::cout << SDL_GetCurrentAudioDriver() << std::endl;
}

DDWAudio& DDWAudio::GetInstance()
{
	static DDWAudio instance{};
	return instance;
}

void DDWAudio::SetBuffer(Uint8* pBuffer)
{
	m_pCurrentSoundBuffer = pBuffer;
}

void DDWAudio::SetLength(Uint32 length)
{
	m_CurrentSoundLength = length;
}

Uint8* DDWAudio::GetCurrentBuffer() const
{
	return m_pCurrentSoundBuffer;
}

Uint32 DDWAudio::GetCurrentLength() const
{
	return m_CurrentSoundLength;
}

void DDWAudio::Initialize()
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return;

	m_AmountOfAudioDevices = SDL_GetNumAudioDevices(0);

	std::cout << "Successfully created and initialized the DAudio engine!" << std::endl;
}