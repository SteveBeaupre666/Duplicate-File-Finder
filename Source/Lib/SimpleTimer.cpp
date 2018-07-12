#include "SimpleTimer.h"

//-----------------------------------------------------------------------------
// Constructor...
//-----------------------------------------------------------------------------
CSimpleTimer::CSimpleTimer()
{
	Reset();
}

//-----------------------------------------------------------------------------
// Reset the timer
//-----------------------------------------------------------------------------
void CSimpleTimer::Reset()
{
	ElapsedTime = 0;
	CurrentTime = GetTickCount();
	LastTime    = CurrentTime;
}

//-----------------------------------------------------------------------------
// Return the elapsed time
//-----------------------------------------------------------------------------
DWORD CSimpleTimer::Tick(bool reset)
{
	if(reset)
		LastTime = CurrentTime;

	CurrentTime = GetTickCount();
	ElapsedTime = CurrentTime - LastTime;

	return ElapsedTime;
}
