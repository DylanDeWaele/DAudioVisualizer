#include "DDWSound.h"
#include <iostream>

#include "DDWAudio.h"


DDWSound::DDWSound(const std::string& filename)
	: m_Filename{ filename },
	m_DataType{},
	m_WAVData{}
{
	LoadSound(filename);
}

DDWSound::~DDWSound()
{

}


WAVFileFormat DDWSound::GetData() const
{
	return m_WAVData;
}


void DDWSound::LoadSound(const std::string& filename)
{
	std::string extension{};

	//1. Find the filenames extension
	extension = GetFileExtension(filename);

	//2. Check the extension and load the corresponding parser function
	if (extension == "wav") 
	{
		m_DataType = DataType::WAV;

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