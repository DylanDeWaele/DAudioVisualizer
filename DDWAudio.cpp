#include "DDWAudio.h"
#include <iostream>
#include "DDWChannel.h"

FMOD_RESULT F_CALLBACK WriteSoundData(FMOD_SOUND*, void* pData, unsigned int length)
{
	//Clear output
	memset(pData, 0, length);

	signed short* pPCMData = (signed short*)pData;

	int pcmDataCount = length / 2;

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
	m_pSystem->release();

	//Delete all channels
	for (DDWChannel* pChannel : m_pChannels)
		delete pChannel;
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

	std::cout << "Successfully initialized FMOD system!" << std::endl;

	//Create vector of channels
	for (unsigned int i = 0; i < m_AmountOfChannels; i++)
	{
		m_pChannels.push_back(new DDWChannel{});
	}

	//Initialize looping sound
	memset(&m_SoundInfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	m_SoundInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);

	m_SoundInfo.defaultfrequency = 44100;
	m_SoundInfo.format = FMOD_SOUND_FORMAT_PCM16;
	m_SoundInfo.numchannels = 2;

	m_SoundInfo.length = 44100 * 2 * sizeof(signed short) * 100; //100 second sound
	m_SoundInfo.decodebuffersize = 4410;

	m_SoundInfo.pcmreadcallback = WriteSoundData;

	//Create a looping sound
	FMOD_MODE mode = FMOD_LOOP_NORMAL | FMOD_OPENUSER;
	m_pSystem->createStream(nullptr, mode, &m_SoundInfo, &m_pSound);
	m_pSystem->playSound(m_pSound, nullptr, false, nullptr);
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
