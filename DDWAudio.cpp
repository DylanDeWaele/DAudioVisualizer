#include "DDWAudio.h"
#include <iostream>
#include "DDWChannel.h"

FMOD_RESULT F_CALLBACK WriteSoundData(FMOD_SOUND*, void* pData, unsigned int length)
{
	//Clear output
	memset(pData, 0, length);

	signed short* pPCMData = (signed short*)pData;

	const unsigned int pcmDataCount = length / 2;

	const unsigned int amountOfChannels = DDWAudio::GetInstance().GetAmountOfChannels();
	const std::vector<DDWChannel*>& pChannels = DDWAudio::GetInstance().GetChannels();

	//Tell every channel to write to the output
	for (unsigned int i = 0; i < amountOfChannels; i++)
	{
		pChannels[i]->WriteSoundData(pPCMData, pcmDataCount);
	}

	return FMOD_OK;
}

DDWAudio::DDWAudio()
	: m_pSystem{ nullptr },
	m_pSound{nullptr},
	m_SoundInfo{},
	m_AmountOfChannels{ 2 }, //2 Channels
	m_pChannels{ }
{
}

DDWAudio::~DDWAudio()
{
	m_pSound->release();

	//Delete all channels
	for (DDWChannel* pChannel : m_pChannels)
		delete pChannel;
	
	m_pSystem->release();
}


DDWAudio& DDWAudio::GetInstance()
{
	static DDWAudio instance{};
	return instance;
}

void DDWAudio::Initialize()
{
	FMOD_RESULT fr = FMOD::System_Create(&m_pSystem);

	if (fr != FMOD_OK)
	{
		std::cout << "Failed to create FMOD system." << std::endl;
		return;
	}

	fr = m_pSystem->init(m_AmountOfChannels, FMOD_INIT_NORMAL, nullptr);

	if (fr != FMOD_OK)
	{
		std::cout << "Failed to initialize FMOD system." << std::endl;
		return;
	}

	for (unsigned int i = 0; i < m_AmountOfChannels; i++)
	{
		m_pChannels.push_back(new DDWChannel{});
	}

	//Initialize FMOD callback loop
	const int sampleRate = 44100;

	m_SoundInfo = { 0 };
	m_SoundInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	m_SoundInfo.format = FMOD_SOUND_FORMAT_PCM16;
	m_SoundInfo.length = sampleRate * m_AmountOfChannels * sizeof(signed short) * 10; //10 second sound buffer
	m_SoundInfo.numchannels = m_AmountOfChannels;
	m_SoundInfo.pcmreadcallback = WriteSoundData;
	m_SoundInfo.defaultfrequency = sampleRate;
	m_SoundInfo.decodebuffersize = (int)(sampleRate * 0.1); //Nr of samples we will be submitting at a time => 100ms for this

	//Create a sound loop
	FMOD_MODE mode = FMOD_LOOP_NORMAL | FMOD_OPENUSER;

	fr = m_pSystem->createStream(nullptr, mode, &m_SoundInfo, &m_pSound);

	if (fr != FMOD_OK)
	{
		std::cout << "Failed to create FMOD stream for the FMOD system." << std::endl;
		return;
	}

	fr = m_pSystem->playSound(m_pSound, nullptr, false, nullptr);

	if (fr != FMOD_OK)
	{
		std::cout << "Failed to start the sound loop of the FMOD system." << std::endl;
		return;
	}

	std::cout << "Successfully initialized FMOD system!" << std::endl;
}

void DDWAudio::Update()
{
	m_pSystem->update();
}

int DDWAudio::GetAmountOfChannels() const
{
	return m_AmountOfChannels;
}

const std::vector<DDWChannel*>& DDWAudio::GetChannels() const
{
	return m_pChannels;
}

DDWChannel* DDWAudio::GetFreeChannel() const
{
	//Loop over the channels and return the first free one
	for (DDWChannel* pChannel : m_pChannels) 
	{
		if (pChannel->IsFree())
			return pChannel;
	}

	std::cout << "No free channel available." << std::endl;
	return nullptr;
}