//---------------------------------------------------------------------------
#pragma hdrstop
#include "FilesList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constructor...
//-----------------------------------------------------------------------------
FilesList::FilesList()
{
	Initialize();
}

//-----------------------------------------------------------------------------
// Destructor...
//-----------------------------------------------------------------------------
FilesList::~FilesList()
{
	Clear();
}

//-----------------------------------------------------------------------------
// Initialise the list
//-----------------------------------------------------------------------------
void FilesList::Initialize()
{
	head = NULL;
	tail = NULL;
	NodesCount = 0;
}

//-----------------------------------------------------------------------------
// Return true if the indx is valid
//-----------------------------------------------------------------------------
bool FilesList::IndexInRange(int indx)
{
	return indx >= 0 && indx < NodesCount;
}

//-----------------------------------------------------------------------------
// Return true if the indx is valid
//-----------------------------------------------------------------------------
bool FilesList::IndexOutOfBounds(int indx)
{
	return indx < 0 || indx > NodesCount;
}

//-----------------------------------------------------------------------------
// Return true if the list is empty
//-----------------------------------------------------------------------------
bool FilesList::IsEmpty()
{
	return NodesCount == 0;
}

//-----------------------------------------------------------------------------
// Return the number of nodes in the list
//-----------------------------------------------------------------------------
int FilesList::GetSize()
{
	return NodesCount;
}

//-----------------------------------------------------------------------------
// Return true if the given pointer is the head node
//-----------------------------------------------------------------------------
bool FilesList::IsHead(NODE *n)
{
	return head == n;
}

//-----------------------------------------------------------------------------
// Return true if the given pointer is the tail node
//-----------------------------------------------------------------------------
bool FilesList::IsTail(NODE *n)
{
	return tail == n;
}

//-----------------------------------------------------------------------------
// Return the node at the given position
//-----------------------------------------------------------------------------
FilesListNode* FilesList::GetNode(int indx)
{
	int i = 0;
	FilesListNode* node = head;

	while(node){

		if(indx == i)
			return node;

		node = node->GetNext();
		i++;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Return the first node of the list
//-----------------------------------------------------------------------------
FilesListNode* FilesList::GetFirstNode()
{
	return head;
}

//-----------------------------------------------------------------------------
// Return the last node of the list
//-----------------------------------------------------------------------------
FilesListNode* FilesList::GetLastNode()
{
	return tail;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FilesListNode* FilesList::Push(String *fname, UI64 fsize)
{
	FilesListNode* new_node = new NODE;
	NodesCount++;

	// No head?
	if(!head){
		head = new_node;
		tail = new_node;
	} else {
		// Update previous and next pointers
		new_node->prev = tail;
		tail->next = new_node;
		// Update the tail
		tail = new_node;
	}

	// Copy the node's data
	new_node->SetFileInfo(fname, fsize);

	return new_node;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool FilesList::Delete(int indx)
{
	// Check for indx out of bounds exception...
	if(!IndexInRange(indx) || IndexOutOfBounds(indx))
		return false;

	// Get prev and next nodes pointers
	FilesListNode* node = GetNode(indx);
	FilesListNode* prev = node->GetPrev();
	FilesListNode* next = node->GetNext();

	// Update prev and next nodes pointers
	if(prev){node->prev->next = next;}
	if(next){node->next->prev = prev;}

	// Update head and tail pointers
	if(IsHead(node)){head = node->next;}
	if(IsTail(node)){tail = node->prev;}

	// Delete the node
	delete node;
	NodesCount--;

	return true;
}

//-----------------------------------------------------------------------------
// Remove the last node of the list
//-----------------------------------------------------------------------------
bool FilesList::Pop()
{
	return Delete(NodesCount - 1);
}

//-----------------------------------------------------------------------------
// Clear the entire list
//-----------------------------------------------------------------------------
void FilesList::Clear()
{
	while(Pop());
}