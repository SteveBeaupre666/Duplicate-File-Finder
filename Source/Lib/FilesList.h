//---------------------------------------------------------------------------
#ifndef FilesListH
#define FilesListH
//----------------------------------------------------------------------//
#include <Windows.h>
#include <System.Classes.hpp>
//----------------------------------------------------------------------//
#ifdef UI64
#undef UI64
#endif
#define UI64 unsigned __int64
//----------------------------------------------------------------------//
#ifdef NODE
#undef NODE
#endif
#define NODE CFilesListNode
//----------------------------------------------------------------------//

class CFilesList;

struct CFileInfo {
	String FileName;
	UI64   FileSize;
	BOOL   Duplicated;
};

class CFilesListNode {
public:
	CFilesListNode(){
		ClearFileInfo();
		prev = next = NULL;
	}
	friend class CFilesList;
private:
	CFileInfo FileInfo;
	NODE *prev, *next;
public:

	void ClearFileInfo(){
		FileInfo.FileSize   = 0;
		FileInfo.FileName   = "";
		FileInfo.Duplicated = FALSE;
	}

	CFileInfo* GetFileInfo(){return &FileInfo;}

	String GetFileName(){return FileInfo.FileName;}
	UI64   GetFileSize(){return FileInfo.FileSize;}
	BOOL   IsDuplicated(){return FileInfo.Duplicated;}

	void SetFileInfo(CFileInfo *fi){
		FileInfo.FileSize   = fi->FileSize;
		FileInfo.FileName   = fi->FileName;
		FileInfo.Duplicated = fi->Duplicated;
	}

	NODE* GetPrev(){return prev;}
	NODE* GetNext(){return next;}
};


class CFilesList {
public:
	CFilesList();
	~CFilesList();
private:
	UI64  size;
	NODE *head, *tail;
private:
	void  Initialize();
	bool  IsIndexValid(UI64 indx);
public:
	UI64  GetSize();
	bool  IsEmpty();

	NODE* GetNode(UI64 indx);
	NODE* GetFirstNode();
	NODE* GetLastNode();

	NODE* Add(CFileInfo *fi);
	bool  Delete(UI64 indx);
	void  Clear();
};
#endif
