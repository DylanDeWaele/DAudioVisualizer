#include <iostream>
#include <string>
#include <Dsound.h>
#include <vector>

//https://edn.embarcadero.com/article/20941

//Struct to keep audio device data in
struct Device
{
	LPGUID guid;
	std::string description;
	std::string module;

	//Ctor
	Device() : guid{ nullptr }, description{  }, module{  }
	{}
	//Dtor
	~Device()
	{
		delete guid;
	}
};

std::string GetFullDescription(LPGUID guid, LPCSTR desc)
{
	//We need to do this small little hack in order to get the full device name, hopefuly the device name ends with a ) 
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
std::string GetFullModule(LPGUID guid, LPCSTR mod)
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

//Enumeration function - this function is called everytime we enumerate over an audio device and then we can check if we should add it to the list or not
BOOL CALLBACK EnumCallBack(LPGUID guid, LPCSTR desc, LPCSTR mod, LPVOID list)
{
	Device* device = new Device{};

	if (guid == nullptr)
		device->guid = nullptr;
	else
		device->guid = guid;

	device->description = GetFullDescription(guid, desc);
	//Fill in the description of the primary sound devicy
	if (device->description.empty())
		device->description = "Primary Audio Device";

	device->module = GetFullModule(guid, mod);

	//Add the device to the list
	(static_cast<std::vector<Device*>*>(list))->push_back(device);
	return true;
}

int main()
{
	//List of audio devices
	std::vector<Device*> audioDevices{};
	unsigned int i{};
	std::string filename{};

	HRESULT hr = DirectSoundEnumerate(LPDSENUMCALLBACK(EnumCallBack), &audioDevices);

	if (FAILED(hr))
		std::cout << "Failed to enumerate over audio devices" << std::endl;


	//Show the available audio devices
	std::cout << "Available Audio Devices: " << std::endl;
	for (i = 0; i < audioDevices.size(); i++)
	{
		std::cout << i << ": " << audioDevices[i]->description << " " << audioDevices[i]->module << std::endl;
	}

	return 1;
}