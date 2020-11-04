#pragma once
#include <iostream>
#include <string>
#include <Dsound.h>
#include <vector>

//DOCUMENTATION:
//https://edn.embarcadero.com/article/20941

//I'm gonna use a singleton pattern so we can access the DAudio engine and its devices anywhere in code
class DAudio
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
	DAudio();

	//Dtor
	~DAudio();

	//Singleton pattern
	static DAudio& GetInstance() 
	{
		static DAudio instance{};

		return instance;
	}

	//Rule of 5
	DAudio(const DAudio& other) = delete; //Copy constructor
	DAudio(DAudio&& other) = delete; //Move constructor
	DAudio& operator=(const DAudio& other) = delete; //Copy assignment
	DAudio& operator=(DAudio&& other) = delete; //Move assignment

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