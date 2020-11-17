#pragma once
#include <string>

#include "Structs.h"

class DDWSound
{
public:
	DDWSound(const std::string& filename);
	~DDWSound() = default;

	DDWSound(const DDWSound& other) = delete;
	DDWSound(DDWSound&& other) = delete;
	DDWSound& operator=(const DDWSound& other) = delete;
	DDWSound& operator=(DDWSound&& other) = delete;

	const WAVFileFormat& GetData() const;

	void Play();

private:
	const std::string m_Filename;

	WAVFileFormat m_WAVData;

	void LoadSound(const std::string& filename);
	std::string GetFileExtension(const std::string& filename);
};