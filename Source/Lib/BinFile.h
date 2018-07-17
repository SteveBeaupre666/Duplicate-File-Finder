#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//
#define UI64 unsigned __int64
//----------------------------------------------------------------------//

class CBinFile {
public:
	CBinFile();
	~CBinFile();
protected:
	HANDLE h;

	void Initialize();
	virtual void InitializeEx(){}

	UI64 Pack64(void *hi, void *lo);
	void Unpack64(UI64 n, void *hi, void *lo);
public:
	bool IsOpened();
	void Close();

	virtual bool Create(char *fname);
	virtual bool Open(char *fname, bool ReadOnly = true);
	
	bool Read (void *buf, UINT size, UINT *num_read = NULL);
	bool Write(void *buf, UINT size, UINT *num_written = NULL);

	virtual UI64 GetSize();
	virtual void Flush();

	UI64 Tell();
	bool Seek(UI64 Offset, UINT MoveMethod);
};
