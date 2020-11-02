#include "DAudio.h"

#pragma region CALLBACKS
//Enumeration function - this function is called every time we enumerate over an audio device and then we can check if we should add it to the list or not
BOOL CALLBACK EnumCallback(LPGUID guid, LPCSTR desc, LPCSTR mod, LPVOID list)
{
	DAudio::Device* device = new DAudio::Device{};

	if (guid == nullptr)
		device->guid = nullptr;
	else
		device->guid = guid;

	device->description = DAudio::GetFullDescription(guid, desc);
	//Fill in the description of the primary sound device
	if (device->description.empty())
		device->description = "Primary Audio Device";

	device->module = DAudio::GetFullModule(guid, mod);

	//Add the device to the list
	(static_cast<std::vector<DAudio::Device*>*>(list))->push_back(device);
	return true;
}
#pragma endregion

DAudio::DAudio()
	: m_Hr{},
	m_pAudioDevices{},
	m_pCurrentAudioDevice{nullptr}
{
}

DAudio::~DAudio()
{
	//Delete all audio devices
	for (auto d : m_pAudioDevices)
	{
		delete d;
	}

	m_pAudioDevices.clear();
}

DAudio::Device* DAudio::GetCurrentAudioDevice() const
{
	return m_pCurrentAudioDevice;
}

void DAudio::PrintAudioDevices() const
{
	//Show the available audio devices
	std::cout << "Available Audio Devices: " << std::endl;
	std::cout << "------------------------ " << std::endl;

	for (unsigned int i = 0; i < m_pAudioDevices.size(); i++)
	{
		std::cout << i << ": " << m_pAudioDevices[i]->description << " " << m_pAudioDevices[i]->module << std::endl;
	}
}

void DAudio::PrintCurrentAudioDevice() const
{
	std::cout << "Current Audio Device: " << std::endl;
	std::cout << "--------------------- " << std::endl;

	std::cout << m_pCurrentAudioDevice->description << " " << m_pCurrentAudioDevice->module << std::endl;
}

void DAudio::Initialize()
{
	m_Hr = DirectSoundEnumerate(LPDSENUMCALLBACK(EnumCallback), &m_pAudioDevices);

	if (FAILED(m_Hr))
		std::cout << "Failed to get audio devices." << std::endl;

	//The current audio device is the primary one by default
	m_pCurrentAudioDevice = m_pAudioDevices[0];
}

void DAudio::SelectAudioDevice(int index)
{
	m_pCurrentAudioDevice = m_pAudioDevices[index];
}

std::string DAudio::GetFullDescription(LPGUID guid, LPCSTR desc)
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
std::string DAudio::GetFullModule(LPGUID guid, LPCSTR mod) 
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