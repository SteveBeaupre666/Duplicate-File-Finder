#include "FileIO.h"

CFileIO::CFileIO()
{
	Initialize();
}
 
CFileIO::~CFileIO()
{
	Close(); 
}

void CFileIO::Initialize()
{
	h = INVALID_HANDLE_VALUE;
}
 
bool CFileIO::IsOpened()
{
	return h != INVALID_HANDLE_VALUE;
}
 
bool CFileIO::Open(wchar_t *fname)
{
	Close();
	h = CreateFileW(fname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return IsOpened();
}
 
void CFileIO::Close()
{
	if(IsOpened()){
		CloseHandle(h);
		Initialize();
	}
}

bool CFileIO::Read(void *buffer, UINT size, UINT *num_read)
{
	/*if(!IsOpened() || size == 0)
		return false;

	DWORD NumBytesRead = 0;
	ReadFile(h, buffer, size, &NumBytesRead, NULL);
	if(num_read)
		*num_read = NumBytesRead;

	return NumBytesRead == size;*/

	return false;
}

