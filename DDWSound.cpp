#include "DDWSound.h"
#include <iostream>

#include "DDWAudio.h"

//https://gist.github.com/armornick/3447121

void AudioCallback(void*, Uint8* pStream, int length)
{
	DDWAudio& audioEngine = DDWAudio::GetInstance();
	Uint32 currentLength = audioEngine.GetCurrentLength();
	Uint8* pCurrentSoundBuffer = audioEngine.GetCurrentBuffer();

	if (audioEngine.GetCurrentLength() <= 0)
		return;

	if ((unsigned int)length > currentLength)
		length = currentLength;

	//SDL_MixAudio(pStream, pCurrentSoundBuffer, length, SDL_MIX_MAXVOLUME); //This gives me weird jittery sound behaviour
	SDL_memcpy (pStream, pCurrentSoundBuffer, length);

	pCurrentSoundBuffer += length; //Increment the position of the current sound buffer
	currentLength -= length;

	audioEngine.SetBuffer(pCurrentSoundBuffer);
	audioEngine.SetLength(currentLength);
}

DDWSound::DDWSound(const std::string& filename)
	: m_Filename{ filename },
	m_Length{},
	m_pBuffer{ nullptr },
	m_Specs{}
{
	LoadSound(filename);
}

DDWSound::~DDWSound()
{
	SDL_FreeWAV(m_pBuffer);
}

void DDWSound::Play()
{
	if (SDL_OpenAudio(&m_Specs, nullptr) < 0)
	{
		std::cout << "Couldn't open audio: " << SDL_GetError() << std::endl;
		return;
	}

	SDL_PauseAudio(0);

	std::cout << "Now playing: " << m_Filename << std::endl;
}

void DDWSound::Stop()
{
	SDL_PauseAudio(1);
}

Uint32 DDWSound::GetLength() const
{
	return m_Length;
}

Uint8* DDWSound::GetBuffer() const
{
	return m_pBuffer;
}

void DDWSound::LoadSound(const std::string& filename)
{
	if (!SDL_LoadWAV(filename.c_str(), &m_Specs, &m_pBuffer, &m_Length))
	{
		std::cout << "Failed to load: " << filename << std::endl;
		return;
	}

	//Set the callback
	m_Specs.callback = AudioCallback;
	m_Specs.userdata = nullptr;

	DDWAudio::GetInstance().SetBuffer(m_pBuffer);
	DDWAudio::GetInstance().SetLength(m_Length);

	std::cout << "Successfully loaded " << filename << std::endl;
}
