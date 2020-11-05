#pragma once
#include <iostream>
#include <string>
#include <Dsound.h>
#include <vector>

//DOCUMENTATION:
//https://edn.embarcadero.com/article/20941

class DDWAudio
{
public:
	struct Device
	{
		LPGUID guid;
		std::string description;
		std::string module;

		//Ctor
		Device() : guid{ nullptr }, description{  }, module{  } { }
		//Dtor
		~Device() { guid = nullptr; }
	};

	//Ctor
	DDWAudio();

	//Dtor
	~DDWAudio();

	//Rule of 5
	DDWAudio(const DDWAudio& other) = delete; //Copy constructor
	DDWAudio(DDWAudio&& other) = delete; //Move constructor
	DDWAudio& operator=(const DDWAudio& other) = delete; //Copy assignment
	DDWAudio& operator=(DDWAudio&& other) = delete; //Move assignment

	//Public member functions
	void Initialize();

	//This functions sets the current audio device to the audio device at the given index
	void SelectAudioDevice(int index);

	//Gets the current active audio device
	Device* GetCurrentAudioDevice() const;
	LPDIRECTSOUND GetDirectSound() const;
	LPDIRECTSOUNDBUFFER GetPrimarySoundBuffer() const;

	void PrintAudioDevices() const;
	void PrintCurrentAudioDevice() const;

	//Using static functions so we can access them inside the CALLBACK function
	static std::string GetFullDescription(LPGUID guid, LPCSTR desc); //Get full description of audio device
	static std::string GetFullModule(LPGUID guid, LPCSTR mod); //Get full module of audio device

private:
	//Private data members
	HRESULT m_Hr;

	std::vector<Device*> m_pAudioDevices;
	Device* m_pCurrentAudioDevice;

	LPDIRECTSOUND m_pDirectSound;

	LPDIRECTSOUNDBUFFER m_pPrimarySoundBuffer;
};