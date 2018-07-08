//---------------------------------------------------------------------------
#pragma hdrstop
#include "FilesList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constructor...
//-----------------------------------------------------------------------------
CFilesList::CFilesList()
{
	Initialize();
}

//-----------------------------------------------------------------------------
// Destructor...
//-----------------------------------------------------------------------------
CFilesList::~CFilesList()
{
	Clear();
}

//-----------------------------------------------------------------------------
// Initialise the list
//-----------------------------------------------------------------------------
void CFilesList::Initialize()
{
	size = 0;
	head = tail = NULL;
}

//-----------------------------------------------------------------------------
// Return the number of nodes in the list
//-----------------------------------------------------------------------------
UI64 CFilesList::GetSize()
{
	return size;
}

//-----------------------------------------------------------------------------
// Return true if the list is empty
//-----------------------------------------------------------------------------
bool CFilesList::IsEmpty()
{
	return !size;
}

//-----------------------------------------------------------------------------
// Return the node at the given position
//-----------------------------------------------------------------------------
CFilesListNode* CFilesList::GetNode(UI64 indx)
{
	UI64 i = 0;
	CFilesListNode* node = head;

	while(node){

		if(indx == i++)
			return node;

		node = node->next;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Return the first node of the list
//-----------------------------------------------------------------------------
CFilesListNode* CFilesList::GetFirstNode()
{
	return head;
}

//-----------------------------------------------------------------------------
// Return the last node of the list
//-----------------------------------------------------------------------------
CFilesListNode* CFilesList::GetLastNode()
{
	return tail;
}

//-----------------------------------------------------------------------------
// Return true if the index is < size
//-----------------------------------------------------------------------------
bool CFilesList::IsIndexValid(UI64 indx)
{
	return indx < size;
}

//-----------------------------------------------------------------------------
// Add a new node at the end of the list
//-----------------------------------------------------------------------------
CFilesListNode* CFilesList::Add(CFileInfo *fi)
{
	CFilesListNode* new_node = new NODE;
	if(!new_node)
		return NULL;

	size++;

	if(!head){
		head = new_node;
		tail = new_node;
	} else {
		new_node->prev = tail;
		tail->next = new_node;
		tail = new_node;
	}

	new_node->SetFileInfo(fi);

	return new_node;
}

//-----------------------------------------------------------------------------
// Delete a node
//-----------------------------------------------------------------------------
bool CFilesList::Delete(UI64 indx)
{
	if(!IsIndexValid(indx))
		return false;

	CFilesListNode* node = GetNode(indx);
	CFilesListNode* prev = node->prev;
	CFilesListNode* next = node->next;

	if(prev){node->prev->next = next;}
	if(next){node->next->prev = prev;}

	if(node == head){head = node->next;}
	if(node == tail){tail = node->prev;}

	delete node;
	size--;

	return true;
}

//-----------------------------------------------------------------------------
// Clear the entire list
//-----------------------------------------------------------------------------
void CFilesList::Clear()
{
	while(Delete(0));
}