#include "DDWSound.h"
#include <iostream>

#include "DDWAudio.h"
#include "DDWChannel.h"
#include "DDWMasterParser.h"

DDWSound::DDWSound(const std::string& filename)
	: m_Filename{ filename },
	m_WAVData{}
{
	LoadSound(filename);
}

const WAVFileFormat& DDWSound::GetData() const
{
	return m_WAVData;
}

void DDWSound::Play()
{
	//Play the sound in a free channel
	DDWAudio::GetInstance().GetFreeChannel()->Play(this);
}

void DDWSound::LoadSound(const std::string& filename)
{
	std::string extension{};

	extension = GetFileExtension(filename);

	if (extension == "wav") 
	{
		if (!DDWMasterParser::GetInstance().LoadWAVFile(filename, m_WAVData))
			std::cout << "Error loading " << filename << std::endl;
	}
}

std::string DDWSound::GetFileExtension(const std::string& filename)
{
	//Find the position of the last dot in the filename
	const size_t& dotPos = filename.find_last_of('.');

	if (dotPos != std::string::npos)
		return filename.substr(dotPos + 1);

	return "";
}