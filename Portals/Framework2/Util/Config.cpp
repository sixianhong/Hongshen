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

#include "Config.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#elif defined(LINUX)
#include <stdio.h>
#endif

Config::Config(){
}

Config::~Config(){
	for (unsigned int i = 0; i < entries.getCount(); i++){
		delete entries[i].name;
	}
}

bool Config::init(){
#if defined(_WIN32)

	HKEY hkey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Humus", 0, KEY_READ, &hkey) == ERROR_SUCCESS){
		char name[256];
		unsigned long type, nameSize = sizeof(name);
		unsigned long value, valueSize;

		unsigned int i = 0;
		while (RegEnumValue(hkey, i, name, &nameSize, NULL, &type, NULL, NULL) != ERROR_NO_MORE_ITEMS){
			if (type == REG_DWORD){
				valueSize = sizeof(value);
				if (RegQueryValueEx(hkey, name, NULL, NULL, (LPBYTE) &value, &valueSize) == ERROR_SUCCESS){
					entries.add(Entry(name, value, false));
				}
			}
			nameSize = sizeof(name);
			i++;
		}
		RegCloseKey(hkey);
		return true;
	}

#elif defined(LINUX)

	char fileName[256];
	sprintf(fileName, "%s/.humusdemos.conf", getenv("HOME"));

	Tokenizer tok(2);
	if (tok.setFile(fileName)){
		char *name, *value;
		while ((name = tok.next()) != NULL){
			if ((value = tok.nextAfterToken("=")) != NULL){
				if ((name[0] >= 'A' && name[0] <= 'Z') || (name[0] >= 'a' && name[0] <= 'z')){
					entries.add(Entry(name, atoi(value), false));
				}
				tok.goToNext();
			}
		}

		return true;
	}
	
#endif

	return false;
}

bool Config::flush(){
	bool created = false;

#if defined(_WIN32)

	HKEY hkey;

	for (unsigned int i = 0; i < entries.getCount(); i++){
		if (entries[i].dirty){
			if (!created){
				if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Humus", NULL, "REG_SZ", REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hkey, NULL) != ERROR_SUCCESS) return false;
				created = true;
			}
			RegSetValueEx(hkey, entries[i].name, 0, REG_DWORD, (unsigned char *) &entries[i].value, sizeof(int));
		}
	}
	if (created) RegCloseKey(hkey);

#elif defined(LINUX)

	FILE *file = NULL;
	unsigned int i = 0;
	while (i < entries.getCount()){
		if (created){
			fprintf(file, "%s = %d;\n", entries[i].name, entries[i].value);
		} else {
			if (entries[i].dirty){
				char fileName[256];
				sprintf(fileName, "%s/.humusdemos.conf", getenv("HOME"));
				file = fopen(fileName, "wb");
				created = true;
				i = 0;
				continue;
			}
		}
		i++;
	}
	if (created) fclose(file);

#endif
	return true;
}

bool Config::getBoolDef(const char *name, bool def) const {
	for (unsigned int i = 0; i < entries.getCount(); i++){
		if (stricmp(entries[i].name, name) == 0){
			return (entries[i].value != 0);
		}
	}
	return def;
}

int Config::getIntegerDef(const char *name, int def) const {
	for (unsigned int i = 0; i < entries.getCount(); i++){
		if (stricmp(entries[i].name, name) == 0){
			return entries[i].value;
		}
	}
	return def;
}


bool Config::getInteger(const char *name, int &dest) const {
	for (unsigned int i = 0; i < entries.getCount(); i++){
		if (stricmp(entries[i].name, name) == 0){
			dest = entries[i].value;
			return true;
		}
	}
	return false;
}

void Config::setBool(const char *name, bool val){
	setInteger(name, val);
}

void Config::setInteger(const char *name, int val){
	for (unsigned int i = 0; i < entries.getCount(); i++){
		if (stricmp(entries[i].name, name) == 0){
			if (entries[i].value != val){
				entries[i].value = val;
				entries[i].dirty = true;
			}
			return;
		}
	}
	entries.add(Entry(name, val, true));
}
