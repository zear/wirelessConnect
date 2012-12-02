#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "font.h"
#include "global.h"
#include "input.h"
#include "logic.h"
#include "network.h"
#include "onScreenKeyboard.h"

MenuContainer *CurrentMenu = NULL;
MenuItem *SelectedItem = NULL;

MenuContainer *MenuMain = NULL;
MenuContainer *MenuOptions = NULL;
MenuContainer *MenuProfiles = NULL;

MenuContainer *menuCreateNew(MenuContainer *Container, int number, char *caption, void *callback)
{
	MenuItem *NewItem = NULL;

	if(Container == NULL)
	{
		Container = malloc(sizeof(MenuContainer));
		if(Container == NULL)
		{
			fprintf(stderr, "ERROR: menuCreateNew: Out of memory!\n");
			return NULL;
		}
		Container->Menu = NULL;
		Container->size = 0;
	}

	NewItem = malloc(sizeof(MenuItem));
	if(NewItem == NULL)
	{
		fprintf(stderr, "ERROR: menuCreateNew: Out of memory!\n");
		return Container; // TODO: instead of returning destroy the container
	}

	NewItem->number = number;
	NewItem->callback = callback;
	strcpy(NewItem->caption, caption);

	NewItem->Next = Container->Menu;
	Container->Menu = NewItem;
	Container->size++;

	return Container;
}

MenuItem *menuSwitchItem(MenuContainer *Container, int number)
{
	MenuItem *NewItem = Container->Menu;

	while(NewItem != NULL)
	{
		if(NewItem->number == number)
		{
			return NewItem;
		}

		NewItem = NewItem->Next;
	}

	return NULL;
}

void actOptions(MenuItem *this)
{
	CurrentMenu = MenuOptions;
	SelectedItem = menuSwitchItem(CurrentMenu, 0);
}

void actConnect(MenuItem *this)
{
	if(CurNetwork.status == STATUS_OFF)
	{
		CurNetwork.status = STATUS_CONNECTING;
		if(!networkConnect())
		{
			CurNetwork.status = STATUS_ON;
		}
		else
		{
			CurNetwork.status = STATUS_FAILED;
		}
	}
	else
	{
		networkDisconnect();
		CurNetwork.status = STATUS_OFF;
	}
}

void actQuit(MenuItem *this)
{
	setGameState(STATE_EXIT);
}

void actOptionsMode(MenuItem *this)
{
	CurNetwork.mode++;
	if(CurNetwork.mode > 2)
	{
		CurNetwork.mode = 0;
	}
}

void actOptionsESSID(MenuItem *this)
{
	Keyboard.enabled = 1;
	Keyboard.inputLen = 59;
	Keyboard.type = OSK_ALPHANUM;
	Keyboard.source = CurNetwork.essid;
}

void actOptionsEnc(MenuItem *this)
{
	CurNetwork.encryption++;
	if(CurNetwork.encryption > 3)
	{
		CurNetwork.encryption = 0;
	}
}

void actOptionsPassword(MenuItem *this)
{
	Keyboard.enabled = 1;
	Keyboard.inputLen = 59;
	Keyboard.type = OSK_ALPHANUM;
	Keyboard.source = CurNetwork.key;
}

void actOptionsDHCP(MenuItem *this)
{
	CurNetwork.dhcp++;
	if(CurNetwork.dhcp > 1)
	{
		CurNetwork.dhcp = 0;
	}
}

void actOptionsIP(MenuItem *this)
{
	Keyboard.enabled = 1;
	Keyboard.inputLen = 14;
	Keyboard.type = OSK_IP;
	Keyboard.source = CurNetwork.ip;
}

void actOptionsNETMASK(MenuItem *this)
{
	Keyboard.enabled = 1;
	Keyboard.inputLen = 14;
	Keyboard.type = OSK_NUMERIC;
	Keyboard.source = CurNetwork.netmask;
}

void actOptionsBack(MenuItem *this)
{
	CurrentMenu = MenuMain;
	SelectedItem = menuSwitchItem(CurrentMenu, 0);
}

void actProfiles(){

}

void menuDeleteSingle(MenuContainer *Container)
{
	MenuItem *CurrentItem;

	if(Container == NULL)
	{
		return;
	}

	CurrentItem = Container->Menu;
	while(CurrentItem != NULL)
	{
		Container->Menu = CurrentItem->Next;
		free(CurrentItem);
		CurrentItem = Container->Menu;
	}
}

void menuDeleteAll()
{
	menuDeleteSingle(MenuMain);
}

void actionStub(MenuItem *this)
{
	printf("%i\n", this->number);
}

void menuLoadAll()
{
	MenuMain = menuCreateNew(MenuMain, 0, "NETWORK SETTINGS", actOptions);
	MenuMain = menuCreateNew(MenuMain, 1, "SELECT PROFILE", actProfiles );
	MenuMain = menuCreateNew(MenuMain, 2, "CONNECT/DISCONNECT", actConnect);
	MenuMain = menuCreateNew(MenuMain, 3, "EXIT", actQuit);

	MenuOptions = menuCreateNew(MenuOptions, 0, "MODE", actOptionsMode);
	MenuOptions = menuCreateNew(MenuOptions, 1, "ESSID", actOptionsESSID);
	MenuOptions = menuCreateNew(MenuOptions, 2, "ENCRYPTION", actOptionsEnc);
	MenuOptions = menuCreateNew(MenuOptions, 3, "PASSWORD", actOptionsPassword);
	//MenuOptions = menuCreateNew(MenuOptions, 4, "DHCP", actOptionsDHCP);
	//MenuOptions = menuCreateNew(MenuOptions, 5, "IP", actOptionsIP);
	//MenuOptions = menuCreateNew(MenuOptions, 6, "NETMASK", actOptionsNETMASK);
	MenuOptions = menuCreateNew(MenuOptions, 4, "", NULL);
	MenuOptions = menuCreateNew(MenuOptions, 5, "BACK", actOptionsBack);

	CurrentMenu = MenuMain;
	SelectedItem = menuSwitchItem(CurrentMenu, 0);
}

void menuInput()
{
	MenuItem *NewItem = NULL;
	int newItemNumber = SelectedItem->number;

	if(!keyDelayCheck(SDLK_UP, 15))
	{
		newItemNumber--;
	}
	else if(!keyDelayCheck(SDLK_DOWN, 15))
	{
		newItemNumber++;
	}

	if(newItemNumber < 0)
	{
		newItemNumber = CurrentMenu->size - 1;
	}

	if(newItemNumber >= CurrentMenu->size)
	{
		newItemNumber = 0;
	}

	if(newItemNumber != SelectedItem->number)
	{
		NewItem = menuSwitchItem(CurrentMenu, newItemNumber);
		if(NewItem != NULL)
		{
			while(NewItem->callback == NULL)
			{
				if(NewItem->number < SelectedItem->number)
				{
					newItemNumber--;
				}
				else if(NewItem->number > SelectedItem->number)
				{
					newItemNumber++;
				}
				else
				{
					return;
				}

				NewItem = menuSwitchItem(CurrentMenu, newItemNumber);
			}

			SelectedItem = NewItem;
		}
	}

	if(keystate[SDLK_RETURN])
	{
		keystate[SDLK_RETURN] = 0;
		//menuAction(SelectedItem);
		SelectedItem->callback(SelectedItem);
	}
	if(keystate[SDLK_LCTRL])
	{
		keystate[SDLK_LCTRL] = 0;
		//menuAction(SelectedItem);
		SelectedItem->callback(SelectedItem);
	}
}

void menuDrawSingle(MenuContainer *Container, int number, int x, int y)
{
	MenuItem *CurrentItem = Container->Menu;

	if(number < 0 || number > Container->size)
	{
		fprintf(stderr, "ERROR (menuDraw): No such menu: %d\n", number);
		return;
	}

	while(CurrentItem != NULL)
	{
		if(CurrentItem->number == number)
		{
			drawText(CurrentItem->caption, x, y, &FontUnifont);

			if(CurrentItem == SelectedItem)
			{
				drawText(">", 60, y, &FontUnifont);
				drawText("<", SCREEN_WIDTH - 60, y, &FontUnifont);
			}
			break;
		}

		CurrentItem = CurrentItem->Next;
	}
}

void menuDraw(MenuContainer *Container, int x, int y)
{
	int i;

	for(i = 0; i < CurrentMenu->size; i++)
	{
		menuDrawSingle(CurrentMenu, i, x, y + i * 15); 
	}
}
