#pragma once
#include <SDL.h>
#include <string>

class DDWSound
{
public:
	DDWSound(const std::string& filename);

	~DDWSound();

	void Play();
	void Stop();

	Uint32 GetLength() const;
	Uint8* GetBuffer() const;

private:
	const std::string m_Filename;
	Uint32 m_Length;
	Uint8* m_pBuffer;
	SDL_AudioSpec m_Specs;

	void LoadSound(const std::string& filename);
};

