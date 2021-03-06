#ifndef _MENU_H_
#define _MENU_H_

typedef struct MenuItemStruct
{
	int number;
	//MenuAction Action;
	void *(*callback) (void *this);
	char caption[21];
	struct MenuItemStruct *Next;
} MenuItem;

typedef struct MenuContainerStruct
{
	MenuItem *Menu;
	int size;
} MenuContainer;

MenuContainer *menuCreateNew(MenuContainer *Container, int number, char *caption, void* callback);
MenuItem *menuSwitchItem(MenuContainer *Container, int number);
void menuAction(MenuItem *Item);
void menuDeleteSingle(MenuContainer *Container);
void menuDeleteAll();
void menuLoadAll();
void menuInput();
void menuDrawSingle(MenuContainer *Container, int number, int x, int y, int sz);
void menuDraw(MenuContainer *Container, int x, int y);

void listProfiles();

void updateIpAddress();

extern MenuContainer *CurrentMenu;
extern MenuItem *SelectedItem;

extern MenuContainer *MenuMain;
extern MenuContainer *MenuOptions;
extern MenuContainer *MenuNetworks;
extern MenuContainer *MenuNetwork;

#endif /* _MENU_H_ */
