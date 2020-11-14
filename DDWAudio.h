#pragma once
#include <string>
#include <vector>
#include <SDL.h>

//Documentation
//https://wiki.libsdl.org/SDL_GetNumAudioDevices

class DDWAudio
{
public:

	DDWAudio();

	~DDWAudio();

	DDWAudio(const DDWAudio& other) = delete;
	DDWAudio(DDWAudio&& other) = delete;
	DDWAudio& operator=(const DDWAudio& other) = delete;
	DDWAudio& operator=(DDWAudio&& other) = delete;

	void Initialize();

	void PrintAudioDevices() const;
	void PrintCurrentAudioDevice() const;

	static DDWAudio& GetInstance();

	void SetBuffer(Uint8* pBuffer);
	void SetLength(Uint32 length);

	Uint8* GetCurrentBuffer() const;
	Uint32 GetCurrentLength() const;

private:
	int m_AmountOfAudioDevices;

	Uint8* m_pCurrentSoundBuffer;
	Uint32 m_CurrentSoundLength;
};