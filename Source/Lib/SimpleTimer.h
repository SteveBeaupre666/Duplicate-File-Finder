#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
//----------------------------------------------------------------------//

//-----------------------------------------------------------------------------
// Simple Timer class
//-----------------------------------------------------------------------------
class CSimpleTimer {
public:
	CSimpleTimer();
protected:
	DWORD LastTime;
	DWORD CurrentTime;
	DWORD ElapsedTime;
public:
	void  Reset();
	DWORD Tick(bool reset = true);
};
