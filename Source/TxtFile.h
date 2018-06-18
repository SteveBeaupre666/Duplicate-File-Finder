#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//

class CTxtFile {
public:
	CTxtFile();
	~CTxtFile();
private:
	FILE *hFile;
private:
	void Initialize();
public:
	bool IsOpened();
	bool Open(wchar_t *fname);
	bool Create(wchar_t *fname);
	void Close();

	bool  ReadChar(wchar_t *c);
	wchar_t* ReadLine(wchar_t *buffer, int size);

	bool WriteChar(wchar_t c);
	int  WriteLine(wchar_t *buffer);
	int  WriteArgs(const wchar_t *fmt, ...);

	int  Tell();
	void Flush();
};
