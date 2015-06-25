/***********      .---.         .-"-.      *******************\
* -------- *     /   ._.       / � ` \     * ---------------- *
* Author's *     \_  (__\      \_�v�_/     * humus@rogers.com *
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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "Tokenizer.h"

struct Entry {
	Entry(const char *eName, int val, bool dirt){
		name = new char[strlen(eName) + 1];
		strcpy(name, eName);
		value = val;
		dirty = dirt;
	}

	char *name;
	int value;
	bool dirty;
};

class Config {
public:
	Config();
	~Config();

	bool init();
	bool flush();

	bool getBoolDef(const char *name, bool def) const;
	int getIntegerDef(const char *name, int def) const;
	bool getInteger(const char *name, int &dest) const;
	void setBool(const char *name, bool val);
	void setInteger(const char *name, int val);
private:

	Set <Entry> entries;
};

#endif // _CONFIG_H_
