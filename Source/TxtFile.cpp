#include "TxtFile.h"

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

CTxtFile::CTxtFile()
{
	Initialize(); 
}
 
//----------------------------------------------------------------------//

CTxtFile::~CTxtFile()
{
	Close(); 
}
 
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CTxtFile::Initialize()
{
	hFile = NULL;
}
 
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

bool CTxtFile::IsOpened()
{
	return hFile != NULL;
}
 
//----------------------------------------------------------------------//

bool CTxtFile::Open(wchar_t *fname)
{
	LPCWSTR Flag = L"rt";

	Close();

	hFile = _wfopen(fname, Flag);

	return IsOpened();
}
 
//----------------------------------------------------------------------//

bool CTxtFile::Create(wchar_t *fname)
{
	LPCWSTR Flag = L"wt+";

	Close();

	hFile = _wfopen(fname, Flag);

	return IsOpened();
}
 
//----------------------------------------------------------------------//

void CTxtFile::Close()
{
	if(IsOpened()){
		fclose(hFile);
		Initialize();
	} 
}
 
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

bool CTxtFile::ReadChar(wchar_t *c)
{
	if(!IsOpened())
		return false;

	int res = fgetwc(hFile);

	if(feof(hFile)){
		*c = 0;
		return false;
	} else {
		*c = (wchar_t)res;
		return true;
	}
}
 
//----------------------------------------------------------------------//

wchar_t* CTxtFile::ReadLine(wchar_t *buffer, int size)
{
	if(!IsOpened())
		return NULL;

	return fgetws(buffer, size, hFile);
}
 
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

bool CTxtFile::WriteChar(wchar_t c)
{
	if(!IsOpened())
		return false;

	int res = fputwc(c, hFile);

	return res != EOF;
}
 
//----------------------------------------------------------------------//

int CTxtFile::WriteLine(wchar_t *buffer)
{
	if(!IsOpened())
		return 0;

	int res = fputws(buffer, hFile);

	return res;
}
 
//----------------------------------------------------------------------//

int CTxtFile::WriteArgs(const wchar_t *fmt, ...)
{
	if(!IsOpened())
		return 0;

	const int MaxSize = 4096;
	const int BufSize = MaxSize * sizeof(wchar_t);

	wchar_t Buffer[MaxSize];
	ZeroMemory(Buffer, BufSize);

	va_list ap;
	va_start(ap, fmt);
	_vsnwprintf(Buffer, MaxSize, fmt, ap);
	va_end(ap);

	return WriteLine(Buffer);
}
 
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

int CTxtFile::Tell()
{
	if(!IsOpened())
		return 0;

	return (int)ftell(hFile);
}
 
//----------------------------------------------------------------------//

void CTxtFile::Flush()
{
	if(IsOpened())
		fflush(hFile); 
}
 

