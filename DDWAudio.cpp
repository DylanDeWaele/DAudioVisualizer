#include "DDWAudio.h"

#pragma region CALLBACKS
//Enumeration function - this function is called every time we enumerate over an audio device and then we can check if we should add it to the list or not
BOOL CALLBACK EnumCallback(LPGUID guid, LPCSTR desc, LPCSTR mod, LPVOID list)
{
	DDWAudio::Device* device = new DDWAudio::Device{};

	if (guid == nullptr)
		device->guid = nullptr;
	else
		device->guid = guid;

	device->description = DDWAudio::GetFullDescription(guid, desc);
	//Fill in the description of the primary sound device
	if (device->description.empty())
		device->description = "Primary Audio Device";

	device->module = DDWAudio::GetFullModule(guid, mod);

	//Add the device to the list
	(static_cast<std::vector<DDWAudio::Device*>*>(list))->push_back(device);
	return true;
}
#pragma endregion

DDWAudio::DDWAudio()
	: m_Hr{},
	m_pAudioDevices{},
	m_pCurrentAudioDevice{nullptr},
	m_pDirectSound{nullptr},
	m_pPrimarySoundBuffer{nullptr}
{
}

DDWAudio::~DDWAudio()
{
	//Delete all audio devices
	for (auto d : m_pAudioDevices)
	{
		delete d;
	}

	m_pAudioDevices.clear();
}

DDWAudio::Device* DDWAudio::GetCurrentAudioDevice() const
{
	return m_pCurrentAudioDevice;
}

LPDIRECTSOUND DDWAudio::GetDirectSound() const
{
	return m_pDirectSound;
}

LPDIRECTSOUNDBUFFER DDWAudio::GetPrimarySoundBuffer() const
{
	return m_pPrimarySoundBuffer;
}

void DDWAudio::PrintAudioDevices() const
{
	//Show the available audio devices
	std::cout << "Available Audio Devices: " << std::endl;
	std::cout << "------------------------ " << std::endl;

	for (unsigned int i = 0; i < m_pAudioDevices.size(); i++)
	{
		std::cout << i << ": " << m_pAudioDevices[i]->description << " " << m_pAudioDevices[i]->module << std::endl;
	}
}

void DDWAudio::PrintCurrentAudioDevice() const
{
	std::cout << "Current Audio Device: " << std::endl;
	std::cout << "--------------------- " << std::endl;

	std::cout << m_pCurrentAudioDevice->description << " " << m_pCurrentAudioDevice->module << std::endl;
}

void DDWAudio::Initialize()
{
	m_Hr = DirectSoundEnumerate(LPDSENUMCALLBACK(EnumCallback), &m_pAudioDevices);

	if (FAILED(m_Hr))
	{
		std::cout << "Failed to get audio devices." << std::endl;
		return;
	}

	//The current audio device is the primary one by default
	m_pCurrentAudioDevice = m_pAudioDevices[0];

	//Create direct sound object
	m_Hr = DirectSoundCreate(m_pCurrentAudioDevice->guid, &m_pDirectSound, nullptr);

	if (FAILED(m_Hr))
	{
		std::cout << "Failed to create DirectSound." << std::endl;
		return;
	}

	//Set the cooperative level
	m_Hr = m_pDirectSound->SetCooperativeLevel(GetDesktopWindow(), DSSCL_NORMAL); //I suppose I need to change this to the window handle?

	if (FAILED(m_Hr))
	{
		std::cout << "Failed to set cooperative level of DirectSound." << std::endl;
		return;
	}

	//Create primary buffer descriptions
	DSBUFFERDESC bufferDesc{};
	WAVEFORMATEX waveFormat{};

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	bufferDesc.dwBufferBytes = 0; //Must be 0 for primary buffer
	bufferDesc.lpwfxFormat = nullptr; //Must be nullptr for primary buffer

	waveFormat.wFormatTag = WAVE_FORMAT_PCM; //Pulse code modulation format
	waveFormat.nChannels = 2; //Stereo
	waveFormat.nSamplesPerSec = 44100; //44100 Hz
	waveFormat.wBitsPerSample = 16; //At 16 bits per same
	waveFormat.nBlockAlign = 4; // From documentation: {If wFormatTag is WAVE_FORMAT_PCM, nBlockAlign should be equal to the product of nChannels and wBitsPerSample divided by 8 (bits per byte).} => ((2*16) / 8 = 4)
	waveFormat.nAvgBytesPerSec = 176400; //From documentation: { If wFormatTag is WAVE_FORMAT_PCM, nAvgBytesPerSec should be equal to the product of nSamplesPerSec and nBlockAlign.} => (44100 * 4 = 176400)

	//Create the primary buffer
	m_Hr = m_pDirectSound->CreateSoundBuffer(&bufferDesc, &m_pPrimarySoundBuffer, nullptr);

	if (FAILED(m_Hr))
	{
		std::cout << "Failed to create primary sound buffer." << std::endl;
		return;
	}

	//Set buffer format
	m_pPrimarySoundBuffer->SetFormat(&waveFormat);

	//Success!
	std::cout << "Successfully created and initialized the DAudio engine!" << std::endl;
}

void DDWAudio::SelectAudioDevice(int index)
{
	m_pCurrentAudioDevice = m_pAudioDevices[index];
}

std::string DDWAudio::GetFullDescription(LPGUID guid, LPCSTR desc)
{
	//We need to do this small little hack in order to get the full device name, hopefully the device name ends with a ) 
	std::string description{};

	if (guid)
	{
		unsigned int i = 0;
		while (true)
		{
			description.append(desc + i);

			if (*(desc + i) == ')')
				break;

			++i;
		}
	}

	return description;
}
std::string DDWAudio::GetFullModule(LPGUID guid, LPCSTR mod)
{
	//We need to do this small little hack in order to get the full module name, normally the module ends with a second }
	std::string module{};

	if (guid)
	{
		unsigned int i = 0;
		unsigned int j = 0;

		while (true)
		{
			module.append(mod + i);

			if (*(mod + i) == '}')
			{
				++j;
				if (j == 2)
					break;
			}
			++i;
		}
	}

	return module;
}