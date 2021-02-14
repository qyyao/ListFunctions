//Annie Yao
//Last updated: Feburary 14th

//Able to create multiple lists up to LIST_MAX_NUM_HEAD
//Each list can hold values up to LIST_MAX_NUM_NODES
//Contains functions to create, edit, and delete these lists


#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "list.h"

static bool firstCreate = true;

static List heads[LIST_MAX_NUM_HEADS];    //create pool of list heads
static Node nodes[LIST_MAX_NUM_NODES];    //create pool of list nodes

//tracking idea: 
//two arrays containing index of the nodes/heads
//index tracker keeps all used nodes/heads's indicies to the left of the tracker, unused to the right
//when a node/head is freed, its index is put in the array before the tracker and tracker is decremented 

static int freeHeads[LIST_MAX_NUM_HEADS];    //array of free list heads
static int freeNodes[LIST_MAX_NUM_NODES];    //array of free list nodes

static int freeHeadIndex = 0;       //index of first free head in freeHeads
static int freeNodeIndex = 0;       //index of first free node in freeNodes

//HELPER FUNCTIONS:
//initializes the first node of the list
static void initializeFirstNode(List * pList, Node * newNode) //initializes the first ever node in a list
{
    pList -> head = newNode;
    pList -> tail = newNode;
    pList -> head -> prev = NULL;
    pList -> tail -> next = NULL;
    pList -> current = newNode;
    pList -> currentPosition = 1;
    pList -> itemCount = 1;
    return;
}

//create a new node from the available nodes
static Node * createNewNode(void * pItem)
{
    if (freeNodeIndex >= LIST_MAX_NUM_NODES) //if all nodes are used
    {
        return NULL;
    }

    Node * newNode = &nodes[freeNodes[freeNodeIndex]]; //take free node from the free nodes list
    freeNodeIndex++;
    newNode -> item = pItem;
    return newNode;
}


//initializes the free trackers upon first call of create_List()
static void initializeFreeArrays()
{
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++) 
    {
        freeHeads[i] = i;
        heads[i].headIndex = i;
    }
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++) 
    {
        freeNodes[i] = i;
        nodes[i].nodeIndex = i;
    }
    return;
}

//adds node to the end of the list
static void addNodeToTail(List * pList, Node * pNode)
{
    pList -> tail -> next = pNode;
    pNode -> prev = pList -> tail;
    pNode -> next = NULL;
    pList -> tail = pNode;
    pList -> current = pNode;
    pList -> currentPosition = 3;
    pList -> itemCount++;
    return;    
}

//adds node to the start of the list
static void addNodeToHead(List * pList, Node * pNode)
{   
    pList -> head -> prev = pNode;
    pNode -> next = pList -> head;
    pNode -> prev = NULL;
    pList -> head = pNode;
    pList -> current = pNode;
    pList -> currentPosition = 1;
    pList -> itemCount++;
    return;
}

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create()
{
    if (firstCreate){       //the very first time we call List_create(), we want to initialize the two pools 
        initializeFreeArrays();
        firstCreate = false;
    }

    if (freeHeadIndex >= LIST_MAX_NUM_HEADS) //if all list heads have been used, return null
    {
        return NULL;
    }
    
    List * newList = &heads[freeHeads[freeHeadIndex]]; //take a free head from the free heads array and increment past its index
    freeHeadIndex++;

    newList -> head = NULL;  //set default initial conditions (safety)
    newList -> tail = NULL;
    newList -> current = NULL;
    newList -> currentPosition = -1;
    newList -> itemCount = 0;

    return newList;
}

// Returns the number of items in pList.
int List_count(List* pList)
{
    return pList -> itemCount;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList)
{
    if (pList -> itemCount == 0)
    {
        return NULL;
    }
    pList -> current = pList -> head; //set current position to head
    pList -> currentPosition = 1;
    return pList -> head -> item;
}
// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList)
{          
    if (pList -> itemCount == 0)
    {
        return NULL;
    }
    pList -> current = pList -> tail; //set current position to tail
    pList -> currentPosition = 3;
    return pList -> tail -> item;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList)
{
    if (pList -> itemCount == 1)
    {
        pList -> currentPosition = 3;
    }

    switch(pList -> currentPosition)
    {
        case 0:                                 //when current is before the list: 
            pList -> current = pList -> head;       //set current to head
            pList -> currentPosition = 1;
            break;
        case 1:                                 //when current is on the head, advance one                            
            pList -> current = pList -> current -> next;
            if (pList -> current == pList -> tail) //we must account for when the next node is a tail node (only 2 nodes in the list)
            {
                pList -> currentPosition = 3;
            }
            else
            {
                pList -> currentPosition = 2;
            }
            break;
        case 2:                                 //when current is in the list, advance                       
            pList -> current = pList -> current -> next;
            if (pList -> current == pList -> tail)  //check if it has advanced to the tail
            {
                pList -> currentPosition = 3;
            }
            break;
        case 3:
            pList -> currentPosition = 4;
            pList -> current = NULL;
        default:                     //default (when no nodes in the list or current is beyond the list)
            return NULL;
    }    
    return pList -> current -> item;                                      
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList)
{
    switch(pList -> currentPosition)
    {
        case 1:     
            pList -> currentPosition = 0; 
            pList -> current = NULL;     
            return NULL;
        case 2:
            pList -> current = pList -> current -> prev;
            if (pList -> current == pList -> head)
            {
                pList -> currentPosition = 1;
            }
            return pList -> current -> item;            
        case 3:  //when list is on the tail, move back one
            if (pList -> current == pList -> head)
            {
                pList -> currentPosition = 0;
                pList -> current = NULL;
                return NULL;
            }
            pList -> currentPosition = 2;
            pList -> current = pList -> current -> prev;
            return pList -> current -> item;
        case 4:                     //when current is beyond the list, set current to tail
            pList -> current = pList -> tail;  
            pList -> currentPosition = 3; 
            return pList -> current -> item;    
        default:    //when current is before list or there are no nodes
            return NULL;        
    }                                          
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList)
{
    if (pList -> current == NULL)
    {
        return NULL;
    }
    return pList -> current -> item;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem)
{   
    if (freeNodeIndex >= LIST_MAX_NUM_NODES)
    {
        return -1;
    }    

    Node * newNode = createNewNode(pItem);

    if (pList -> itemCount == 1) //when there is only one node, we want it to prepend to tail, not head
    {
        pList -> currentPosition = 3;       
    }

    switch (pList -> currentPosition)
    {
        case -1: //when no nodes are yet in the list
            initializeFirstNode(pList, newNode);
            break;
        case 0: //when current is before the list    
            addNodeToHead(pList, newNode);
            break;
        case 1: //when current is in the list/head
        case 2:
            newNode -> next = pList -> current -> next;
            pList -> current -> next = newNode; 
            newNode -> next -> prev = newNode;
            newNode -> prev = pList -> current;
            pList -> current = newNode;    
            pList -> itemCount++;    
            break;    
        case 3: //when current is after the list or on tail
        case 4:
            addNodeToTail(pList, newNode);
            break;
    }
    return 0;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem)
{
    if (freeNodeIndex >= LIST_MAX_NUM_NODES)
    {
        return -1;
    }    

    Node * newNode = createNewNode(pItem);

    if (pList -> itemCount == 1) //when there is only one node, we want to prepend to head
    {
        pList -> currentPosition = 1;       
    }    

    switch (pList -> currentPosition)
    {
        case -1: //when there are no nodes
            initializeFirstNode(pList, newNode);
            break;
        case 0:
        case 1:
            addNodeToHead(pList, newNode); //when it's on the head, prepend to head
            break;
        case 2: 
        case 3:  //if not on head or node, insert         
            newNode -> prev = pList -> current -> prev;
            pList -> current -> prev = newNode;
            newNode -> next = pList -> current;
            newNode -> prev -> next = newNode;
            pList -> itemCount++;
            break;
        case 4:
            addNodeToTail(pList, newNode); //when it's on the tail, append to tail
            break;
    }
    return 0;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem)
{
    if (freeNodeIndex >= LIST_MAX_NUM_NODES) //when no free nodes, return 
    {
        return -1;
    }    

    Node * newNode = createNewNode(pItem);

    switch (pList -> currentPosition)
    {
        case -1:
            initializeFirstNode(pList, newNode);
            break;
        default:
            addNodeToTail(pList, newNode);
    }

    return 0;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem)
{
    if (freeNodeIndex >= LIST_MAX_NUM_NODES) //when no free nodes
    {
        return -1;
    }    

    Node * newNode = createNewNode(pItem);

    switch (pList -> currentPosition)
    {
        case -1:
            initializeFirstNode(pList, newNode);
            break;
        default:
            addNodeToHead(pList, newNode); 
    }
    return 0;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList)
{
    Node * tempNode = pList -> current;

    if (pList -> itemCount == 1) //if there is only one node, we can simply remove it
    {
        pList -> currentPosition = -1;
    }
    else
    {
        switch (pList -> currentPosition)
        {
            case 1:
                pList -> head = pList -> head -> next;
                pList -> head -> prev = NULL;
                pList -> current = pList -> head;
                pList -> currentPosition = 1;
                break;
            case 2:
                pList -> current = pList -> current -> next;
                pList -> current -> prev = tempNode -> prev;
                tempNode -> prev -> next = pList -> current;

                if (pList -> current == pList -> tail)
                {
                    pList -> currentPosition = 3;
                }
                break;
            case 3:
                pList -> tail = pList -> tail -> prev;
                pList -> tail -> next = NULL;
                pList -> currentPosition = 4;
                pList -> current = NULL;
                break;
            default: //when current is not in the list, return null
                return NULL;
        }
    }
    pList -> itemCount--;
    freeNodeIndex--; //put the node back into pool of free nodes
    freeNodes[freeNodeIndex] = tempNode -> nodeIndex;
    return tempNode -> item;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2)
{
    if (pList1 -> itemCount == 0) //if first list is empty, just copy second list
    {
        pList1 = pList2;
    }
    else
    {
        if (pList2 -> itemCount != 0) //if second list is empty, do nothing
        {
            pList1 -> tail -> next = pList2 -> head;
            pList2 -> head -> prev = pList1 -> tail;

            pList1 -> tail = pList2 -> tail;
            pList1 -> itemCount += pList2 -> itemCount; 
        }  
    }

    freeHeadIndex--;
    freeHeads[freeHeadIndex] = pList2 -> headIndex;
    return;
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn)
{
    pList -> current = pList -> head;

    while (pList -> current != NULL) //go through each node and free it
    {
        freeNodeIndex--;
        freeNodes[freeNodeIndex] = pList -> current -> nodeIndex;
        (*pItemFreeFn)(pList -> current -> item);

        pList -> current = pList -> current -> next;
    }

    pList -> current = NULL; //reset initial conditions before returning list
    pList -> head = NULL;
    pList -> tail = NULL;
    pList -> currentPosition = -1;
    pList -> itemCount = 0;

    freeHeadIndex--; //put released head back into pool of heads
    freeHeads[freeHeadIndex] = pList -> headIndex;
    return;
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList)
{
    if (pList -> itemCount == 0)
    {
        return NULL;
    }

    freeNodeIndex--;
    freeNodes[freeNodeIndex] = pList -> tail -> nodeIndex; //put the tail back into pool of free nodes

    if (pList -> itemCount == 1) //if there is only one node, set position to -1 and current to null
    {
        pList -> currentPosition = -1;
        pList -> current = NULL;
    }
    else
    {
        pList -> tail = pList -> tail -> prev;
        pList -> current = pList -> tail;
        pList -> currentPosition = 3;
        pList -> tail -> next = NULL;
    }
    
    pList -> itemCount--; 
    return nodes[freeNodes[freeNodeIndex]].item;
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg)
{
    switch (pList -> currentPosition)
    {
        case 0:
            pList -> current = pList -> head; //if before list, then start at head
        case 1:
        case 2: 
        case 3:
            while (pList -> current != NULL)
            {   
                if ((*pComparator)(pList -> current -> item, pComparisonArg))
                {
                    if (pList -> current == pList -> tail) //SET NEW CURRENT POSITION
                    {
                        pList -> currentPosition = 3;
                    }
                    if (pList -> current == pList -> head)
                    {
                        pList -> currentPosition = 1;
                    }
                    else
                    {
                        pList -> currentPosition = 2;
                    }
                    
                    return pList -> current -> item;
                }
                pList -> current = pList -> current -> next;
            }
            break;
        case -1:
        case 4: return NULL;
    }
    //if traverses whole list without finding a match, return NULL and set current to beyond the list
    pList -> currentPosition = 4;
    return NULL;
}
