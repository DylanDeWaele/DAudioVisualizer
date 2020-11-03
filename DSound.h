#pragma once
#include <dsound.h>

class DSound
{
public:
	//Ctor
	DSound();

	//Dtor
	~DSound();

	//Rule of 5
	DSound(const DSound& other) = delete; //Copy constructor
	DSound(DSound&& other) = delete; //Move constructor
	DSound& operator=(const DSound& other) = delete; //Copy assignment
	DSound& operator=(DSound&& other) = delete; //Move assignment
	
	//Public member functions
	LPDIRECTSOUND GetDirectSound();

private:
	//Private data members
	HRESULT m_Hr;
	LPDIRECTSOUND m_pDirectSound;
	HWND m_WindowHandle;

	void Initialize();
};

