/***********      .---.         .-"-.      *******************\
* -------- *     /   ._.       / ´ ` \     * ---------------- *
* Author's *     \_  (__\      \_°v°_/     * humus@rogers.com *
*   note   *     //   \\       //   \\     * ICQ #47010716    *
* -------- *    ((     ))     ((     ))    * ---------------- *
*          ****--""---""-------""---""--****                  ********\
* This file is a part of the work done by Humus. You are free to use  *
* the code in any way you like, modified, unmodified or copy'n'pasted *
* into your own work. However, I expect you to respect these points:  *
*  @ If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  @ For use in anything commercial, please request my approval.      *
*  @ Share your work and ideas too as much as you can.                *
\*********************************************************************/

#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#ifndef NULL
#define NULL 0
#endif

template <class ARG_TYPE>
class ListNode {
public:
	ARG_TYPE object;
	ListNode *prev;
	ListNode *next;
};

template <class ARG_TYPE>
class LinkedList {
protected:
	unsigned int size;
	ListNode <ARG_TYPE> *first, *last;
public:
	LinkedList(){
		size = 0;
		first = last = NULL;
	}

	~LinkedList(){
		clear();
	}

	void clear(){
		ListNode <ARG_TYPE> *node = first;

		while (node != NULL){
			ListNode <ARG_TYPE> *delNode = node;
			node = node->next;
			delete delNode;
		}
		size = 0;
		first = last = NULL;
	}

	const unsigned int getCount() const { return size; }

	ListNode <ARG_TYPE> *getFirst() const { return first; }
	ListNode <ARG_TYPE> *getLast()  const { return last;  }

	void addObjectFirst(const ARG_TYPE &object){
		ListNode <ARG_TYPE> *node = new ListNode <ARG_TYPE>;
		node->object = object;
		addNodeFirst(node);
	}

	void addObjectLast(const ARG_TYPE &object){
		ListNode <ARG_TYPE> *node = new ListNode <ARG_TYPE>;
		node->object = object;
		addNodeLast(node);
	}

	void addNodeFirst(ListNode <ARG_TYPE> *node){
		if (first == NULL){
			first = node;
			last  = node;
			node->prev = NULL;
			node->next = NULL;
		} else {
			first->prev = node;
			node->next = first;
			first = node;
			node->prev = NULL;
		}

		size++;
	}

	void addNodeLast(ListNode <ARG_TYPE> *node){
		if (last == NULL){
			first = node;
			last  = node;
			node->prev = NULL;
			node->next = NULL;
		} else {
			last->next = node;
			node->prev = last;
			last = node;
			node->next = NULL;
		}

		size++;
	}

	void removeNode(ListNode <ARG_TYPE> *node){
		if (node->prev == NULL){
			first = node->next;			
		} else {
			node->prev->next = node->next;
		}
		if (node->next == NULL){
			last = node->prev;
		} else {
			node->next->prev = node->prev;
		}

		delete node;
		size--;
	}
};

#endif // _LINKEDLIST_H_
