#include "DSound.h"
#include "DAudio.h"

DSound::DSound()
{
	Initialize();
}

DSound::~DSound()
{
}

LPDIRECTSOUND DSound::GetDirectSound()
{
	return LPDIRECTSOUND();
}

void DSound::Initialize()
{
	//Create sound using the current audio device of the audio engine
	m_Hr = DirectSoundCreate(DAudio::GetInstance().GetCurrentAudioDevice()->guid, &m_pDirectSound, nullptr);

	if (FAILED(m_Hr))
		std::cout << "Failed to create DirectSound." << std::endl;

	//Set the cooperative level
	m_Hr = m_pDirectSound->SetCooperativeLevel(m_WindowHandle, DSSCL_NORMAL);

	if (FAILED(m_Hr))
		std::cout << "Failed to set cooperative level of DirectSound." << std::endl;
}
