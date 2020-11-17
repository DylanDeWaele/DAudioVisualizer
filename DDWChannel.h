#pragma once

class DDWSound;

class DDWChannel
{
public:
	DDWChannel();

	void WriteSoundData(signed short* pData, int count);
	
	void Play(DDWSound* pSound);
	void Stop();

	void SetVolume(float volume);

	bool IsFree() const;

private:
	DDWSound* m_pCurrentSound;
	unsigned int m_Position;

	float m_Volume;
};

