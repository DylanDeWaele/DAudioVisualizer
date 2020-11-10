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

	bool SetVolume(float volume);
	bool SetPosition(int position);

private:
	const std::string m_Filename;

	void LoadSound(const std::string& filename);
};

