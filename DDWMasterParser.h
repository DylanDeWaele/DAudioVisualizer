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
		unsigned long chunkSize;
		char fileFormat[4];

		//The "fmt" sub-chunk
		char subChunkID[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;

		//The data sub-chunk
		char dataChunkID[4];
		unsigned long dataSize;
		signed short* pData;
	};

	DDWMasterParser() = default;

	~DDWMasterParser() = default;

	static DDWMasterParser& GetInstance();

	bool LoadAudioFile(const std::string& filename, signed short* soundBuffer);

private:
	std::string GetFileExtension(const std::string& filename);

	bool LoadWAVFile(const std::string& filename, signed short* soundBuffer);
	bool VerifyWAVFileIntegrity(std::ifstream& file, WAVFileFormat& wavFile);
	bool CreateWAVFileSoundBuffer(std::ifstream& file, const WAVFileFormat& wavFile, signed short* soundBuffer);
};

