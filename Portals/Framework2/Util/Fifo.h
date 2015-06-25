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

#ifndef _FIFO_H_
#define _FIFO_H_

#ifndef NULL
#define NULL 0
#endif

template <class ARG_TYPE>
class FifoNode {
public:
	ARG_TYPE object;
	FifoNode *next;
};

template <class ARG_TYPE>
class Fifo {
protected:
	unsigned int size;
	FifoNode <ARG_TYPE> *first, *last;
public:
	Fifo(){
		size = 0;
		first = last = NULL;
	}

	~Fifo(){
		clear();
	}

	void clear(){
		while (dequeue());
	}

	const unsigned int getSize() const { return size; }

	const ARG_TYPE &getFirst(){ return first->object; }
	const ARG_TYPE &getLast() { return last->object;  }

	void enqueue(const ARG_TYPE &object){
		FifoNode <ARG_TYPE> *node = new FifoNode <ARG_TYPE>;
		node->object = object;
		node->next = NULL;

		if (size){
			last->next = node;
			last = node;
		} else {
			first = last = node;
		}

		size++;
	}

	bool dequeue(){
		if (size){
			FifoNode <ARG_TYPE> *tmp = first;

			first = first->next;
			delete tmp;

			size--;
			return true;
		}
		return false;
	}

};

#endif // _FIFO_H_
