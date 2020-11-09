#pragma once
#include <DSound.h>
#include <string>

class DDWSound
{
public:
	DDWSound(const std::string& filename);

	~DDWSound();

	bool Play();
	bool Stop();

	bool SetVolume(LONG volume);
	bool SetPosition(DWORD position);

private:
	const std::string m_Filename;
	LPDIRECTSOUNDBUFFER m_pSoundBuffer;

	void LoadSound(const std::string& filename);
};

