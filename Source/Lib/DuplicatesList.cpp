//---------------------------------------------------------------------------
#pragma hdrstop
#include "DuplicatesList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constructor...
//-----------------------------------------------------------------------------
CDuplicatesList::CDuplicatesList()
{
	Initialize();
}

//-----------------------------------------------------------------------------
// Destructor...
//-----------------------------------------------------------------------------
CDuplicatesList::~CDuplicatesList()
{
	Clear();
}

//-----------------------------------------------------------------------------
// Initialise the list
//-----------------------------------------------------------------------------
void CDuplicatesList::Initialize()
{
	size = 0;
	head = tail = NULL;
}

//-----------------------------------------------------------------------------
// Return the number of nodes in the list
//-----------------------------------------------------------------------------
UI64 CDuplicatesList::GetSize()
{
	return size;
}

//-----------------------------------------------------------------------------
// Return true if the list is empty
//-----------------------------------------------------------------------------
bool CDuplicatesList::IsEmpty()
{
	return !size;
}

//-----------------------------------------------------------------------------
// Return the node at the given position
//-----------------------------------------------------------------------------
CDuplicatesListNode* CDuplicatesList::GetNode(UI64 indx)
{
	UI64 i = 0;
	CDuplicatesListNode* node = head;

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
CDuplicatesListNode* CDuplicatesList::GetFirstNode()
{
	return head;
}

//-----------------------------------------------------------------------------
// Return the last node of the list
//-----------------------------------------------------------------------------
CDuplicatesListNode* CDuplicatesList::GetLastNode()
{
	return tail;
}

//-----------------------------------------------------------------------------
// Return true if the index is < size
//-----------------------------------------------------------------------------
bool CDuplicatesList::IsIndexValid(UI64 indx)
{
	return indx < size;
}

//-----------------------------------------------------------------------------
// Add a new node at the end of the list
//-----------------------------------------------------------------------------
CDuplicatesListNode* CDuplicatesList::Add(CFileInfo *fi, UI64 id)
{
	CDuplicatesListNode* new_node = new NODE;
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

	new_node->SetGroupID(id);
	new_node->SetFileInfo(fi);

	return new_node;
}

//-----------------------------------------------------------------------------
// Delete a node
//-----------------------------------------------------------------------------
bool CDuplicatesList::Delete(UI64 indx)
{
	if(!IsIndexValid(indx))
		return false;

	CDuplicatesListNode* node = GetNode(indx);
	CDuplicatesListNode* prev = node->prev;
	CDuplicatesListNode* next = node->next;

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
void CDuplicatesList::Clear()
{
	while(Delete(0));
}