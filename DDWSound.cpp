#include "DDWSound.h"
#include <iostream>

#include "DDWMasterParser.h"

DDWSound::DDWSound(const std::string& filename)
	: m_Filename{filename},
	m_pSoundBuffer{nullptr}
{
	LoadSound(filename);

	//Set default values
	SetPosition(0);
	SetVolume(DSBVOLUME_MAX);
}

DDWSound::~DDWSound()
{
	m_pSoundBuffer->Release();
}

bool DDWSound::Play()
{
	SetPosition(0);

	HRESULT hr = m_pSoundBuffer->Play(0, 0, 0);
	
	if (FAILED(hr))
	{
		std::cout << "Failed to play: " << m_Filename << std::endl;
		return false;
	}

	return true;
}

bool DDWSound::Stop()
{
	HRESULT hr = m_pSoundBuffer->Stop();

	if (FAILED(hr))
	{
		std::cout << "Failed to stop: " << m_Filename << std::endl;
		return false;
	}

	return true;
}

bool DDWSound::SetVolume(LONG volume)
{
	HRESULT hr = m_pSoundBuffer->SetVolume(volume);

	if (FAILED(hr))
	{
		std::cout << "Failed to set volume for: " << m_Filename << std::endl;
		return false;
	}

	return true;
}

bool DDWSound::SetPosition(DWORD position)
{
	HRESULT hr = m_pSoundBuffer->SetCurrentPosition(position);

	if (FAILED(hr))
	{
		std::cout << "Failed to set position for: " << m_Filename << std::endl;
		return false;
	}

	return true;
}

void DDWSound::LoadSound(const std::string& filename)
{
	DDWMasterParser::GetInstance().LoadAudioFile(filename, &m_pSoundBuffer);
}
