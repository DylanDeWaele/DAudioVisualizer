#include "DDWChannel.h"
#include "DDWSound.h"

DDWChannel::DDWChannel()
	:m_pCurrentSound{ nullptr },
	m_Position{},
	m_Volume{ 1.f }
{
}

void DDWChannel::Play(DDWSound* pSound)
{
	m_pCurrentSound = pSound;
	m_Position = 0;
}

void DDWChannel::Stop()
{
	m_pCurrentSound = nullptr;
}

void DDWChannel::SetVolume(float volume)
{
	//Clamp the volume between 1 and 0
	if (volume > 1.f)
		volume = 1.f;
	else if (volume < 0.f)
		volume = 0.f;

	m_Volume = volume;
}

bool DDWChannel::IsFree() const
{
	return m_pCurrentSound == nullptr;
}

void DDWChannel::WriteSoundData(signed short* pData, int count)
{
	if (!m_pCurrentSound)
		return;

	//Write count samples to the data array by steps of 2
	for (int i = 0; i < count; i += 2)
	{
		//If we reached end of the sound stop and return
		if (m_Position >= m_pCurrentSound->GetData().dataSizeDivTwo)
		{
			Stop();
			return;
		}

		//Read value from sound data at current position
		float value = m_pCurrentSound->GetData().pData[m_Position] * m_Volume;

		//Write to left and right channels
		pData[i] = (signed short)(value + pData[i]);
		pData[i + 1] = (signed short)(value + pData[i + 1]);

		//Advance the position by 2 samples, since the L and R channel have already been written
		m_Position += 2;
	}
}