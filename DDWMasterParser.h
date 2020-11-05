#pragma once
#include <string>
#include <dsound.h>
#include <fstream>

//Used this site as help to parse a WAV file
//http://www.rastertek.com/dx11tut14.html

class DDWMasterParser
{
public:
	struct WAVFileFormat
	{
		//The "RIFF" chunk descriptor
		char chunkID[4];
		unsigned int chunkSize;
		char fileFormat[4];

		//The "fmt" sub-chunk
		char subChunkID[4];
		unsigned int subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned int sampleRate;
		unsigned int bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;

		//The data sub-chunk
		char dataChunkID[4];
		unsigned int dataSize;
	};

	//Ctor
	DDWMasterParser() = default;

	//Dtor
	~DDWMasterParser() = default;

	//Singleton pattern
	static DDWMasterParser& GetInstance();

	//Public member functions
	bool LoadAudioFile(const std::string& filename, LPDIRECTSOUNDBUFFER* soundBuffer);

private:

	//Helper functions
	std::string GetFileExtension(const std::string& filename);

	//WAV functions
	bool LoadWAVFile(const std::string& filename, LPDIRECTSOUNDBUFFER* soundBuffer);
	bool VerifyWAVFileIntegrity(std::ifstream& file, WAVFileFormat& wavFile);
	bool CreateWAVFileSoundBuffer(std::ifstream& file, const WAVFileFormat& wavFile, LPDIRECTSOUNDBUFFER* soundBuffer);
};

