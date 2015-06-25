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

#include "Globals.h"
#include "Menu.h"
#include <stdio.h>

MenuItem::MenuItem(const char *capt, void *dataPointer, InputType inpType, int minVal, int maxVal, char **strList){
	subMenu = NULL;
	data = dataPointer;
	type = inpType;
	checked = false;
	stringList = strList;
	min = minVal;
	max = maxVal;

	unsigned int len = strlen(capt);
	caption = new char[len + 1];
	strcpy(caption, capt);
	if (dataPointer != NULL){
		if (inpType == INPUT_KEY){
			char *charString = getKeyString(*(unsigned int *) dataPointer);
			if (charString != NULL) setInputString(charString);
		} else if (inpType == INPUT_BOOL){
			setInputString(*(bool *) dataPointer? "True" : "False");
		} else if (inpType == INPUT_INTEGER){
			char str[16];
			if (stringList){
				setInputString(stringList[*(int *) dataPointer]);
			} else {
				sprintf(str, "%d", *(int *) dataPointer);
				setInputString(str);
			}
		}
	}
}

MenuItem::~MenuItem(){
	delete caption;
	if (subMenu != NULL) delete subMenu;
}

void MenuItem::setInputString(const char *string){
	unsigned int len = strrchr(caption, ':') - caption;
	char *newCaption = new char[len + strlen(string) + 3];

	strncpy(newCaption, caption, len + 2);
	strcpy(newCaption + len + 2, string);

	delete caption;
	caption = newCaption;
}

/* ---------------------------------------------------------------- */

Menu::Menu(){
	parentMenu = NULL;
	currentItem = 0;
}

Menu::~Menu(){
	for (unsigned int i = 0; i < items.getCount(); i++){
		delete items[i];
	}
}

MenuID Menu::addMenuItem(const char *caption, void *data, InputType inputType, int minVal, int maxVal, char **stringList){
	MenuItem *item = new MenuItem(caption, data, inputType, minVal, maxVal, stringList);

	return items.add(item);
}

void Menu::addSubMenu(const MenuID item, Menu *subMenu){
	items[item]->subMenu = subMenu;
	subMenu->parentMenu = this;
}

void Menu::setExclusiveItemChecked(const MenuID item){
	for (unsigned int i = 0; i < items.getCount(); i++){
		items[i]->checked = (i == item);
	}
}

void Menu::setInputKey(const unsigned int charCode){
	char *charString = getKeyString(charCode);
	if (charString != NULL){
		*(unsigned int *) items[currentItem]->data = charCode;
		items[currentItem]->setInputString(charString);
	} else {
		items[currentItem]->setInputString(getKeyString(*(unsigned int *) items[currentItem]->data));
	}
}

void Menu::setInputWait(){
	items[currentItem]->setInputString("-?-");
}

void Menu::prevValue(){
	prevValue(currentItem);
}

void Menu::nextValue(){
	nextValue(currentItem);
}

void Menu::prevValue(const MenuID item){
	if (items[item]->type == INPUT_BOOL){
		bool *data = (bool *) items[item]->data;
		*data = !*data;
		items[item]->setInputString(*data? "True" : "False");
	} else if (items[item]->type == INPUT_INTEGER){
		int *data = (int *) items[item]->data;
		(*data)--;
		if (*data < items[item]->min) *data = items[item]->min;
		if (items[item]->stringList){
			items[item]->setInputString(items[item]->stringList[*data - items[item]->min]);
		} else {
			char str[16];
			sprintf(str, "%d", *data);
			items[item]->setInputString(str);
		}
	}
}

void Menu::nextValue(const MenuID item){
	if (items[item]->type == INPUT_BOOL){
		bool *data = (bool *) items[item]->data;
		*data = !*data;
		items[item]->setInputString(*data? "True" : "False");
	} else if (items[item]->type == INPUT_INTEGER){
		int *data = (int *) items[item]->data;
		(*data)++;
		if (*data > items[item]->max) *data = items[item]->max;
		if (items[item]->stringList){
			items[item]->setInputString(items[item]->stringList[*data - items[item]->min]);
		} else {
			char str[16];
			sprintf(str, "%d", *data);
			items[item]->setInputString(str);
		}
	}
}

void Menu::nextItem(){
	currentItem++;
	if (currentItem >= items.getCount()) currentItem = 0;
}

void Menu::prevItem(){
	if (currentItem == 0) currentItem = items.getCount();
	currentItem--;
}

/* ---------------------------------------------------------------- */

MenuSystem::MenuSystem(){
	currentMenu = mainMenu = new Menu();
}

MenuSystem::~MenuSystem(){
	delete mainMenu;
}

bool MenuSystem::goSubMenu(){
	Menu *subMenu = currentMenu->getCurrentItemSubMenu();
	if (subMenu == NULL) return false;
	currentMenu = subMenu;
	return true;
}

bool MenuSystem::stepUp(){
	Menu *parent = currentMenu->getParentMenu();
	if (parent == NULL) return false;
	currentMenu = parent;
	return true;
}
