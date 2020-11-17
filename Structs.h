#pragma once

struct WAVFileFormat
{
public:
	~WAVFileFormat()
	{
		delete[] pData;
	}

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
	unsigned int dataSizeDivTwo;
	signed short* pData;
};