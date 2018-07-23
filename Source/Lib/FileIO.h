#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
//----------------------------------------------------------------------//

class CFileIO {
public:
	CFileIO();
	~CFileIO();
private:
	HANDLE h;
	void Initialize();
public:
	bool IsOpened();
	bool Open(wchar_t *fname);
	bool Read(void *buf, UINT size, UINT *num_read = NULL);
	void Close();
};
