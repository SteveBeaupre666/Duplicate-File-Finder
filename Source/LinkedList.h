#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//

#define NODE LinkedListNode<T>

//-----------------------------------------------------------------------------
// Linked list node class
//-----------------------------------------------------------------------------
template <class T> class LinkedListNode {
public:
	LinkedListNode(){prev = next = NULL;}
public:
	T item;
	NODE *prev, *next;
};

//-----------------------------------------------------------------------------
// Linked list class
//-----------------------------------------------------------------------------
template <class T> class LinkedList {
public:
	LinkedList();
	~LinkedList();
private:
	int   NodesCount;
	NODE *head, *tail;
private:
	void  Initialize();
	bool  IndexInRange(int index);
	bool  IndexOutOfBounds(int index);
private:
	bool  IsHead(NODE* n);
	bool  IsTail(NODE* n);
public:
	int   GetSize();
	bool  IsEmpty();

	NODE* GetNode(int index);
	NODE* GetFirstNode();
	NODE* GetLastNode();
	
	//bool  NodeExist(NODE* n);
	//int   FindNode(NODE* n);

	//T*    GetItem(int index);
	//bool  SetItem(int index, T *item);

	//int   FindItem(T *item);
	//bool  ItemExist(T *item);

	NODE* Push(T *item);
	NODE* Insert(int index, T *item);

	bool  Delete(int index);
	bool  Pop();

	void  Clear();
public:
	virtual void CopyNodeData(NODE* node, T *data);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Constructor...
//-----------------------------------------------------------------------------
template <class T> LinkedList<T>::LinkedList()
{
	Initialize();
}

//-----------------------------------------------------------------------------
// Destructor...
//-----------------------------------------------------------------------------
template <class T> LinkedList<T>::~LinkedList()
{
	Clear();
}

//-----------------------------------------------------------------------------
// Initialise the list
//-----------------------------------------------------------------------------
template <class T> void LinkedList<T>::Initialize()
{
	head = NULL;
	tail = NULL;
	NodesCount = 0;
}

//-----------------------------------------------------------------------------
// Return true if the index is valid
//-----------------------------------------------------------------------------
template <class T> bool LinkedList<T>::IndexInRange(int index)
{
	return index >= 0 && index < NodesCount;
}

//-----------------------------------------------------------------------------
// Return true if the index is valid
//-----------------------------------------------------------------------------
template <class T> bool LinkedList<T>::IndexOutOfBounds(int index)
{
	return index < 0 || index > NodesCount;
}

//-----------------------------------------------------------------------------
// Return true if the list is empty
//-----------------------------------------------------------------------------
template <class T> bool LinkedList<T>::IsEmpty()
{
	return NodesCount == 0;
}

//-----------------------------------------------------------------------------
// Return the number of nodes in the list
//-----------------------------------------------------------------------------
template <class T> int LinkedList<T>::GetSize()
{
	return NodesCount;
}

//-----------------------------------------------------------------------------
// Return true if the given pointer is the head node
//-----------------------------------------------------------------------------
template <class T> bool LinkedList<T>::IsHead(NODE* n)
{
	return head == n;
}

//-----------------------------------------------------------------------------
// Return true if the given pointer is the tail node
//-----------------------------------------------------------------------------
template <class T> bool LinkedList<T>::IsTail(NODE* n)
{
	return tail == n;
}

//-----------------------------------------------------------------------------
// Return the node at the given position
//-----------------------------------------------------------------------------
template <class T> LinkedListNode<T>* LinkedList<T>::GetNode(int index)
{
	int i = 0;
	NODE* node = head;
	
	while(node){
	
		if(index == i)
			return node;
		
		node = node->next;
		i++;	
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Return the first node of the list
//-----------------------------------------------------------------------------
template <class T> LinkedListNode<T>* LinkedList<T>::GetFirstNode()
{
	return head;
}

//-----------------------------------------------------------------------------
// Return the last node of the list
//-----------------------------------------------------------------------------
template <class T> LinkedListNode<T>* LinkedList<T>::GetLastNode()
{
	return tail;
}

//-----------------------------------------------------------------------------
// Return true if the given node can be found in the list
//-----------------------------------------------------------------------------
/*template <class T> bool LinkedList<T>::NodeExist(NODE* n)
{
	NODE* node = head;

	while(node){

		if(node == n)
			return true;

		node = node->Next();
	}

	return false;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
template <class T> int LinkedList<T>::FindNode(NODE* n)
{
	int i = 0;
	NODE* node = head;

	while(node){

		if(node == n)
			return i;

		node = node->Next();
		i++;	
	}

	return -1;
}

//-----------------------------------------------------------------------------
// Return the node at given position
//-----------------------------------------------------------------------------
template <class T> T* LinkedList<T>::GetItem(int index)
{
	if(IndexInRange(index)){
		NODE* node = GetNode(index);
		if(node)
			return node->GetItemPtr();
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
template <class T> bool LinkedList<T>::SetItem(int index, T *item)
{
	if(IndexInRange(index)){
		NODE* node = GetNode(index);
		if(node){
			node->SetItem(item);
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// new
//-----------------------------------------------------------------------------
template <class T> int LinkedList<T>::FindItem(T& item)
{
	int i = 0;
	NODE* node = head;
	
	while(node){

		if(node->GetItem() == item)
			return i;

		node = node->Next();
		i++;	
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
template <class T> bool LinkedList<T>::ItemExist(T& item)
{
	return FindItem(item) >= 0;
}*/

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
template <class T> LinkedListNode<T>* LinkedList<T>::Push(T *item)
{
	NODE* new_node = new NODE;
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
	CopyNodeData(new_node, item);

	return new_node;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
template <class T> LinkedListNode<T>* LinkedList<T>::Insert(int index, T *item)
{
	if(IndexOutOfBounds(index))
		return NULL;

	// Get a pointer to the node where we want to insert, if any
	NODE* node = GetNode(index);
	if(!node)
		return Push(item);

	// Allocate the new node
	NODE* new_node = new NODE;
	NodesCount++;
	
	// Update new node pointers
	new_node->next = node;
	new_node->prev = node ? node->prev : NULL;
	
	// Create next and previous node pointer
	NODE* prev_node = new_node->prev;
	NODE* next_node = new_node->next;

	// Update next and previous node pointer
	if(prev_node){prev_node->next = new_node;}
	if(next_node){next_node->prev = new_node;}

	// Update head and tail pointers
	if(!new_node->prev){head = new_node;}
	if(!new_node->next){tail = new_node;}

	// Copy the node's data
	CopyNodeData(new_node, item);

	// Return the newly allocated node
	return new_node;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
template <class T> bool LinkedList<T>::Delete(int index)
{
	// Check for index out of bounds exception...
	if(!IndexInRange(index) || IndexOutOfBounds(index))
		return false;

	// Get prev and next nodes pointers
	NODE* node = GetNode(index);
	NODE* prev = node->prev;
	NODE* next = node->next;

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
template <class T> bool LinkedList<T>::Pop()
{
	return Delete(NodesCount - 1);
}

//-----------------------------------------------------------------------------
// Clear the entire list
//-----------------------------------------------------------------------------
template <class T> void LinkedList<T>::Clear()
{
	while(Pop());
}

//-----------------------------------------------------------------------------
// Default node's data copy mechanism
//-----------------------------------------------------------------------------
template <class T> void LinkedList<T>::CopyNodeData(NODE* node, T *data)
{
	node->item = *data;
}







