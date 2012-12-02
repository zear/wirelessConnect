#ifndef _MENU_H_
#define _MENU_H_

typedef enum MenuActionEnum
{
	ACTION_NONE,
	ACTION_PROFILES,
	ACTION_OPTIONS,
	ACTION_CONNECT,
	ACTION_QUIT,
	ACTION_OPTIONS_MODE,
	ACTION_OPTIONS_ESSID,
	ACTION_OPTIONS_ENCRYPTION,
	ACTION_OPTIONS_PASSWORD,
	ACTION_OPTIONS_DHCP,
	ACTION_OPTIONS_IP,
	ACTION_OPTIONS_NETMASK,
	ACTION_OPTIONS_BACK
} MenuAction;

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
void menuDrawSingle(MenuContainer *Container, int number, int x, int y);
void menuDraw(MenuContainer *Container, int x, int y);

void updateIpAddress();

extern MenuContainer *CurrentMenu;
extern MenuItem *SelectedItem;

extern MenuContainer *MenuMain;
extern MenuContainer *MenuOptions;

#endif /* _MENU_H_ */
