#include "DDWMasterParser.h"
#include "DDWAudio.h"
#include <iostream>
#include <dsound.h> //Needed for WAVE_FORMAT_PCM

DDWMasterParser& DDWMasterParser::GetInstance()
{
	static DDWMasterParser instance{};
	return instance;
}

//This function loads in a WAV file and copies its data into the given buffer
bool DDWMasterParser::LoadWAVFile(const std::string& filename, WAVFileFormat& wavFile)
{
	std::ifstream file{};

	file.open(filename, std::ios::in | std::ios::binary);

	//Check if the file was successfully opened
	if (!file.is_open())
	{
		std::cout << "Failed to open: " << filename << std::endl;
		return false;
	}

	//Verify the integrity of the WAV file
	if (!ParseWAVFile(file, wavFile))
	{
		std::cout << "Failed to verify integrity of: " << filename << std::endl;
		return false;
	}

	file.close();

	std::cout << "Successfully loaded and verified the integrity of: " << filename << std::endl;

	return true;
}

//This function will fill in the format of the WAV file AND verify if its correct or not
bool DDWMasterParser::ParseWAVFile(std::ifstream& file, WAVFileFormat& wavFile)
{
	file.seekg(0);

	//Verify the chunk ID is in the RIFF format
	file.read((char*)&wavFile.chunkID, sizeof(wavFile.chunkID));
	if (wavFile.chunkID[0] != 'R' || wavFile.chunkID[1] != 'I' || wavFile.chunkID[2] != 'F' || wavFile.chunkID[3] != 'F')
		return false;

	file.read((char*)&wavFile.chunkSize, sizeof(wavFile.chunkSize));

	//Verify the file format is in the WAVE format
	file.read((char*)&wavFile.fileFormat, sizeof(wavFile.fileFormat));
	if (wavFile.fileFormat[0] != 'W' || wavFile.fileFormat[1] != 'A' || wavFile.fileFormat[2] != 'V' || wavFile.fileFormat[3] != 'E')
		return false;

	//Verify if subChunkID is in the FMT format
	file.read((char*)&wavFile.subChunkID, sizeof(wavFile.subChunkID));
	if (wavFile.subChunkID[0] != 'f' || wavFile.subChunkID[1] != 'm' || wavFile.subChunkID[2] != 't')
		return false;

	file.read((char*)&wavFile.subChunkSize, sizeof(wavFile.subChunkSize));

	//Verify the audio format is WAVE_FORMAT_PCM
	file.read((char*)&wavFile.audioFormat, sizeof(wavFile.audioFormat));
	if (wavFile.audioFormat != WAVE_FORMAT_PCM)
		return false;

	//Check if the WAV file was recorded in stereo format
	file.read((char*)&wavFile.numChannels, sizeof(wavFile.numChannels));
	if (wavFile.numChannels != 2)
		return false;

	//Check if the WAV file was recorded at a sample rate of 44100Hz
	file.read((char*)&wavFile.sampleRate, sizeof(wavFile.sampleRate));
	if (wavFile.sampleRate != 44100)
		return false;

	file.read((char*)&wavFile.bytesPerSecond, sizeof(wavFile.bytesPerSecond));
	file.read((char*)&wavFile.blockAlign, sizeof(wavFile.blockAlign));

	//Check if the WAV file was recorded in a 16 bits per sample format
	file.read((char*)&wavFile.bitsPerSample, sizeof(wavFile.bitsPerSample));
	if (wavFile.bitsPerSample != 16)
		return false;

	//Verify if the dataChunkID is equal to 'data'
	file.read((char*)&wavFile.dataChunkID, sizeof(wavFile.dataChunkID));
	if (wavFile.dataChunkID[0] != 'd' || wavFile.dataChunkID[1] != 'a' || wavFile.dataChunkID[2] != 't' || wavFile.dataChunkID[3] != 'a')
		return false;

	file.read((char*)&wavFile.dataSize, sizeof(wavFile.dataSize));

	//Divide by two to get the length of 1 channel
	wavFile.dataSizeDivTwo = wavFile.dataSize / 2;
	wavFile.pData = new signed short[wavFile.dataSizeDivTwo];

	file.read((char*)wavFile.pData, wavFile.dataSize);

	return true;
}
