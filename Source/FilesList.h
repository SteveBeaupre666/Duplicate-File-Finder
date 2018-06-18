//---------------------------------------------------------------------------
#ifndef FilesListH
#define FilesListH
//----------------------------------------------------------------------//
#include <Windows.h>
#include <System.Classes.hpp>
//----------------------------------------------------------------------//

#define UI64 unsigned __int64
#define NODE FilesListNode

class FilesList;

struct FileInfo {
	String FileName;
	UI64 FileSize;
	BOOL Duplicated;
};

class FilesListNode {
public:
	FilesListNode(){
		prev = next = NULL;
		Info.FileName = "";
		Info.FileSize = 0;
		Info.Duplicated = FALSE;
	}
private:
	FileInfo Info;
	FilesListNode *prev, *next;
public:
	friend class FilesList;

	FileInfo* GetFileInfo(){
		return &Info;
	}

	void SetFileInfo(String *fname, UI64 fsize, BOOL duplicated = FALSE){
		Info.FileSize = fsize;
		Info.FileName = *fname;
		Info.Duplicated = duplicated;
	}

	FilesListNode* GetPrev(){return prev;}
	FilesListNode* GetNext(){return next;}
};


class FilesList {
public:
	FilesList();
	~FilesList();
private:
	int   NodesCount;
	NODE *head, *tail;
private:
	void  Initialize();
	bool  IndexInRange(int indx);
	bool  IndexOutOfBounds(int indx);
private:
	bool  IsHead(NODE* n);
	bool  IsTail(NODE* n);
public:
	int   GetSize();
	bool  IsEmpty();

	NODE* GetNode(int indx);
	NODE* GetFirstNode();
	NODE* GetLastNode();

	NODE* Push(String *fname, UI64 fsize);
	bool  Delete(int indx);
	bool  Pop();
	void  Clear();
};
#endif
