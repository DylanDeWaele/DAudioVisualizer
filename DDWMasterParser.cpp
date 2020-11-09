#include "DDWMasterParser.h"
#include "DDWAudio.h"
#include <iostream>

DDWMasterParser& DDWMasterParser::GetInstance()
{
	static DDWMasterParser instance{};
	return instance;
}

bool DDWMasterParser::LoadAudioFile(const std::string& filename, LPDIRECTSOUNDBUFFER* soundBuffer)
{
	std::string extension{};

	//1. Find the filenames extension
	extension = GetFileExtension(filename);

	//2. Check the extension and load the corresponding helper function
	if (extension == "wav")
		return LoadWAVFile(filename, soundBuffer);

	return false;
}

std::string DDWMasterParser::GetFileExtension(const std::string& filename)
{
	//Find the position of the last dot in the filename
	const size_t& dotPos = filename.find_last_of('.');

	if (dotPos != std::string::npos)
		return filename.substr(dotPos + 1);

	return "";
}

//This function loads in a WAV file and copies its data into the given buffer
bool DDWMasterParser::LoadWAVFile(const std::string& filename, LPDIRECTSOUNDBUFFER* soundBuffer)
{
	std::ifstream file{};
	WAVFileFormat wavFile{};

	//1. Open the file for reading
	file.open(filename, std::ifstream::in | std::ifstream::binary);

	//2. Check if the file was successfully opened
	if (!file.is_open())
	{
		std::cout << "Failed to open: " << filename << std::endl;
		return false;
	}

	//3. Verify the integrity of the WAV file
	if (!VerifyWAVFileIntegrity(file, wavFile))
	{
		std::cout << "Failed to verify integrity of: " << filename << std::endl;
		return false;
	}

	std::cout << "Successfully verified the integrity of: " << filename << std::endl;

	//4. Create the sound buffer
	if (!CreateWAVFileSoundBuffer(file, wavFile, soundBuffer))
	{
		std::cout << "Failed to create soundbuffer of: " << filename << std::endl;
		return false;
	}

	std::cout << "Successfully created soundbuffer of: " << filename << std::endl;

	return true;
}

//This function will fill in the format of the WAV file AND verify if its correct or not
bool DDWMasterParser::VerifyWAVFileIntegrity(std::ifstream& file, WAVFileFormat& wavFile)
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

	return true;
}

bool DDWMasterParser::CreateWAVFileSoundBuffer(std::ifstream& file, const WAVFileFormat& wavFile, LPDIRECTSOUNDBUFFER* soundBuffer)
{
	HRESULT hr{};
	WAVEFORMATEX waveFormat{};
	DSBUFFERDESC bufferDesc{};
	LPDIRECTSOUNDBUFFER tempBuffer = nullptr;
	unsigned char* waveData = nullptr;
	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize{};

	//1. FIll in the wave format of the secondary buffer
	waveFormat.wFormatTag = wavFile.audioFormat;
	waveFormat.nSamplesPerSec = wavFile.sampleRate;
	waveFormat.wBitsPerSample = wavFile.bitsPerSample;
	waveFormat.nChannels = wavFile.numChannels;
	waveFormat.nBlockAlign = wavFile.blockAlign;
	waveFormat.nAvgBytesPerSec = wavFile.bytesPerSecond;
	waveFormat.cbSize = 0;

	//2. Fill in the buffer description of the secondary buffer
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = wavFile.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//3. Create a temporary buffer, if this succeeds then we use this temporary buffer to create the secondary buffer
	hr = DDWAudio::GetInstance().GetDirectSound()->CreateSoundBuffer(&bufferDesc, &tempBuffer, nullptr);
	if (FAILED(hr))
		return false;

	//4. Test the buffer format against the direct sound interface and create the secondary buffer
	hr = tempBuffer->QueryInterface(IID_IDirectSoundBuffer, (void**)&*soundBuffer);
	if (FAILED(hr))
		return false;

	//5. Release the temp buffer
	tempBuffer->Release();

	//6. Start reading in the wav file contents (at offset 44)
	file.seekg(44);

	//7. Create a temporary buffer to hold the contents
	waveData = new unsigned char[wavFile.dataSize]{};
	if (!waveData)
		return false;

	//8. Read the content of the wave file into the buffer
	file.read((char*)waveData, wavFile.dataSize);

	//9. Close the file once done reading
	file.close();
	if (file.is_open())
		return false;

	//10. Lock the secondary buffer and write wave data into it
	hr = (*soundBuffer)->Lock(0, wavFile.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, nullptr, nullptr, 0);
	if (FAILED(hr))
		return false;

	if (bufferSize != wavFile.dataSize)
		return false;

	//11. Copy the data into the buffer
	memcpy(bufferPtr, waveData, wavFile.dataSize);

	//12. Unlock the secondary buffer
	hr = (*soundBuffer)->Unlock((void*)bufferPtr, bufferSize, nullptr, 0);
	if (FAILED(hr))
		return false;

	//13. Release the wave data since it was copied into the secondary buffer
	delete[] waveData;

	return true;
}
