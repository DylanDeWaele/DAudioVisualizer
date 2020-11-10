#include "DDWSound.h"
#include <iostream>

#include "DDWMasterParser.h"

DDWSound::DDWSound(const std::string& filename)
	: m_Filename{filename}
{
	LoadSound(filename);

	//Set default values
	SetVolume(1.f);
	SetPosition(0);
}

DDWSound::~DDWSound()
{
}

bool DDWSound::Play()
{
	SetPosition(0);

	return true;
}

bool DDWSound::Stop()
{
	return true;
}

bool DDWSound::SetVolume(float volume)
{

	return true;
}

bool DDWSound::SetPosition(int position)
{
	return true;
}

void DDWSound::LoadSound(const std::string& filename)
{
}
