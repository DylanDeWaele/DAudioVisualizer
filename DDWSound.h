#pragma once
#include <string>

#include "Structs.h"
#include "DDWMasterParser.h"


class DDWSound
{
public:
	enum class DataType 
	{
		None,
		WAV
	};

	DDWSound(const std::string& filename);

	~DDWSound();

	WAVFileFormat GetData() const;

private:
	const std::string m_Filename;
	DataType m_DataType;

	//Depending on the data type, we have to fill in 1 of the below structs 
	WAVFileFormat m_WAVData; //Contains all data from the wav file like sampling rate, number of channels etc.

	void LoadSound(const std::string& filename);
	std::string GetFileExtension(const std::string& filename);
};