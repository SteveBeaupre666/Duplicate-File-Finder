//---------------------------------------------------------------------------
#ifndef DuplicatesListH
#define DuplicatesListH
//----------------------------------------------------------------------//
#include <Windows.h>
#include <System.Classes.hpp>
//----------------------------------------------------------------------//
#include <FilesList.h>
//----------------------------------------------------------------------//
#ifdef UI64
#undef UI64
#endif
#define UI64 unsigned __int64
//----------------------------------------------------------------------//
#ifdef NODE
#undef NODE
#endif
#define NODE CDuplicatesListNode
//----------------------------------------------------------------------//

class CDuplicatesList;

class CDuplicatesListNode {
public:
	CDuplicatesListNode(){
		GroupID  = 0;
		FileInfo = NULL;
		prev = next = NULL;
	}
private:
	UI64       GroupID;
	CFileInfo *FileInfo;
private:
	NODE *prev, *next;
public:
	friend class CDuplicatesList;

	UI64       GetID(){return GroupID;}
	CFileInfo* GetFileInfo(){return FileInfo;}

	void SetGroupID(UI64 id){GroupID = id;}
	void SetFileInfo(CFileInfo* fi){FileInfo = fi;}

	NODE* GetPrev(){return prev;}
	NODE* GetNext(){return next;}
};


class CDuplicatesList {
public:
	CDuplicatesList();
	~CDuplicatesList();
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

	NODE* Add(CFileInfo *fi, UI64 id);
	bool  Delete(UI64 indx);
	void  Clear();
};
#endif
