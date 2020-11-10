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

	DDWAudio();

	~DDWAudio();

	//Rule of 5
	DDWAudio(const DDWAudio& other) = delete; //Copy constructor
	DDWAudio(DDWAudio&& other) = delete; //Move constructor
	DDWAudio& operator=(const DDWAudio& other) = delete; //Copy assignment
	DDWAudio& operator=(DDWAudio&& other) = delete; //Move assignment

	void Initialize(int index = 0);

	//This functions sets the current audio device to the audio device at the given index
	void SelectAudioDevice(int index);

	void PrintAudioDevices() const;
	void PrintCurrentAudioDevice() const;

	static DDWAudio& GetInstance();

private:
};