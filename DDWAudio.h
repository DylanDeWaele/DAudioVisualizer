#pragma once
#include <string>
#include <vector>
#include <fmod.hpp>

class DDWChannel;

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
	void Update();

	static DDWAudio& GetInstance();

	int GetAmountOfChannels() const;
	const std::vector<DDWChannel*>& GetChannels() const;

	DDWChannel* GetFreeChannel() const;

private:
	FMOD::System* m_pSystem;

	//Variables to create the looping stream we will write data into
	FMOD_CREATESOUNDEXINFO m_SoundInfo;
	FMOD::Sound* m_pSound;
	
	const unsigned int m_AmountOfChannels;
	std::vector<DDWChannel*> m_pChannels;
};