#pragma once
#include <string>
#include <fstream>
#include "DDWSound.h"

//Used this site as help to parse a WAV file
//http://www.rastertek.com/dx11tut14.html

class DDWMasterParser
{
public:
	DDWMasterParser() = default;

	~DDWMasterParser() = default;

	static DDWMasterParser& GetInstance();

	bool LoadWAVFile(const std::string& filename, WAVFileFormat& wavFile);

private:
	bool ParseWAVFile(std::ifstream& file, WAVFileFormat& wavFile);
};

