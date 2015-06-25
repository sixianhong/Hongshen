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

#ifndef _MENU_H_
#define _MENU_H_

#include "Util/Set.h"

typedef unsigned int MenuID;

enum InputType { INPUT_KEY, INPUT_BOOL, INPUT_INTEGER };


class Menu;

class MenuItem {
public:
	MenuItem(const char *capt, void *dataPointer, InputType inpType, int minVal, int maxVal, char **strList);
	~MenuItem();

	void setInputString(const char *string);

	char *caption;
	Menu *subMenu;
	void *data;

	InputType type;
	int min, max;
	char **stringList;
	bool checked;
};

/* ---------------------------------------------------------------- */

class Menu {
public:
	Menu();
	~Menu();

	MenuID addMenuItem(const char *caption, void *data = NULL, InputType inputType = INPUT_KEY, int minVal = 0, int maxVal = 1, char **stringList = NULL);
	void addSubMenu(const MenuID item, Menu *subMenu);

	unsigned int getItemCount() const { return items.getCount(); }
	unsigned int getCurrentItem() const { return currentItem; }
	Menu *getCurrentItemSubMenu() const { return items[currentItem]->subMenu; }
	char *getItemString(const MenuID item) const { return items[item]->caption; }
	char *getCurrentItemString() const { return items[currentItem]->caption; }

	bool isItemChecked(const MenuID item) const { return items[item]->checked; }
	void setItemChecked(const MenuID item, const bool check){ items[item]->checked = check; }
	void setExclusiveItemChecked(const MenuID item);

	void setInputKey(const unsigned int charCode);
	void setInputWait();
	bool isCurrentItemInput() const { return (items[currentItem]->data != NULL); }
	InputType getCurrentInputType() const { return items[currentItem]->type; }
	void prevValue();
	void nextValue();
	void prevValue(const MenuID item);
	void nextValue(const MenuID item);

	void nextItem();
	void prevItem();

	Menu *getParentMenu() const { return parentMenu; }

protected:
	Set <MenuItem *> items;
	Menu *parentMenu;
	unsigned int currentItem;
};

/* ---------------------------------------------------------------- */

class MenuSystem {
public:
	MenuSystem();
	~MenuSystem();

	Menu *getMainMenu() const { return mainMenu; }
	Menu *getCurrentMenu() const { return currentMenu; }

	char *getCurrentItemString() const { return currentMenu->getCurrentItemString(); }

	bool goSubMenu();
	bool stepUp();

protected:
	Menu *mainMenu;
	Menu *currentMenu;
};



#endif // _MENU_H_
