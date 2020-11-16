#pragma once

class DDWSound;

class DDWChannel
{
public:
	DDWChannel();

	void Play(DDWSound* pSound);
	void Stop();

	void WriteSoundData(signed short* pData, int count);

private:
	DDWSound* m_pCurrentSound;
	unsigned int m_Position;
};

