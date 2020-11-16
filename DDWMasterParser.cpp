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

	//1. Open the file for reading
	file.open(filename, std::ifstream::in | std::ifstream::binary);

	//2. Check if the file was successfully opened
	if (!file.is_open())
	{
		std::cout << "Failed to open: " << filename << std::endl;
		return false;
	}

	//3. Verify the integrity of the WAV file
	if (!ParseWAVFile(file, wavFile))
	{
		std::cout << "Failed to verify integrity of: " << filename << std::endl;
		return false;
	}

	std::cout << "Successfully loaded and verified the integrity of: " << filename << std::endl;

	return true;
}

//This function will fill in the format of the WAV file AND verify if its correct or not
bool DDWMasterParser::ParseWAVFile(std::ifstream& file, WAVFileFormat& wavFile)
{
	//Set file cursor position at 0
	file.seekg(0);

	//1. Verify the chunk ID is in the RIFF format
	file.read((char*)&wavFile.chunkID, sizeof(wavFile.chunkID));
	if (wavFile.chunkID[0] != 'R' || wavFile.chunkID[1] != 'I' || wavFile.chunkID[2] != 'F' || wavFile.chunkID[3] != 'F')
		return false;

	//2. Read in the chunkSize
	file.read((char*)&wavFile.chunkSize, sizeof(wavFile.chunkSize));

	//3. Verify the file format is in the WAVE format
	file.read((char*)&wavFile.fileFormat, sizeof(wavFile.fileFormat));
	if (wavFile.fileFormat[0] != 'W' || wavFile.fileFormat[1] != 'A' || wavFile.fileFormat[2] != 'V' || wavFile.fileFormat[3] != 'E')
		return false;

	//4. Verify if subChunkID is in the FMT format
	file.read((char*)&wavFile.subChunkID, sizeof(wavFile.subChunkID));
	if (wavFile.subChunkID[0] != 'f' || wavFile.subChunkID[1] != 'm' || wavFile.subChunkID[2] != 't')
		return false;

	//5. Read in the subChunkSize
	file.read((char*)&wavFile.subChunkSize, sizeof(wavFile.subChunkSize));

	//6. Verify the audio format is WAVE_FORMAT_PCM
	file.read((char*)&wavFile.audioFormat, sizeof(wavFile.audioFormat));
	if (wavFile.audioFormat != WAVE_FORMAT_PCM)
		return false;

	//7. Check if the WAV file was recorded in stereo format
	file.read((char*)&wavFile.numChannels, sizeof(wavFile.numChannels));
	if (wavFile.numChannels != 2)
		return false;

	//8. Check if the WAV file was recorded at a sample rate of 44100Hz
	file.read((char*)&wavFile.sampleRate, sizeof(wavFile.sampleRate));
	if (wavFile.sampleRate != 44100)
		return false;

	//9. Read in the bytes per second value
	file.read((char*)&wavFile.bytesPerSecond, sizeof(wavFile.bytesPerSecond));

	//10. Read in the block align value
	file.read((char*)&wavFile.blockAlign, sizeof(wavFile.blockAlign));

	//11. Check if the WAV file was recorded in a 16 bits per sample format
	file.read((char*)&wavFile.bitsPerSample, sizeof(wavFile.bitsPerSample));
	if (wavFile.bitsPerSample != 16)
		return false;

	//11. Verify if the dataChunkID is equal to 'data'
	file.read((char*)&wavFile.dataChunkID, sizeof(wavFile.dataChunkID));
	if (wavFile.dataChunkID[0] != 'd' || wavFile.dataChunkID[1] != 'a' || wavFile.dataChunkID[2] != 't' || wavFile.dataChunkID[3] != 'a')
		return false;

	//12. Read in the data chunk size
	file.read((char*)&wavFile.dataSize, sizeof(wavFile.dataSize));

	//Divide by two to get the length of 1 channel
	wavFile.dataSizeDivTwo = wavFile.dataSize / 2;

	wavFile.pData = new signed short[wavFile.dataSizeDivTwo];

	//13. Read in the data
	file.read((char*)wavFile.pData, wavFile.dataSize);

	return true;
}
