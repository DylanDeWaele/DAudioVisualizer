#pragma once

struct WAVFileFormat
{
public:
	~WAVFileFormat()
	{
		if (pData)
			delete[] pData;
	}
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
	unsigned long dataSizeDivTwo;
	signed short* pData;
};