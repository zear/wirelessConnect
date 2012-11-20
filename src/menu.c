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

MenuContainer *menuCreateNew(MenuContainer *Container, int number, char *caption, MenuAction Action)
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
	NewItem->Action = Action;
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

void menuAction(MenuItem *Item)
{
	switch(Item->Action)
	{
		case ACTION_OPTIONS:
			CurrentMenu = MenuOptions;
			SelectedItem = menuSwitchItem(CurrentMenu, 0);
			break;
		case ACTION_CONNECT:
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
			break;
		case ACTION_QUIT:
			setGameState(STATE_EXIT);
			break;
		case ACTION_OPTIONS_MODE:
			CurNetwork.mode++;
			if(CurNetwork.mode > 2)
			{
				CurNetwork.mode = 0;
			}
			break;
		case ACTION_OPTIONS_ESSID:
			Keyboard.enabled = 1;
			Keyboard.inputLen = 59;
			Keyboard.type = OSK_ALPHANUM;
			Keyboard.source = CurNetwork.essid;
			break;
		case ACTION_OPTIONS_ENCRYPTION:
			CurNetwork.encryption++;
			if(CurNetwork.encryption > 3)
			{
				CurNetwork.encryption = 0;
			}
			break;
		case ACTION_OPTIONS_PASSWORD:
			Keyboard.enabled = 1;
			Keyboard.inputLen = 59;
			Keyboard.type = OSK_ALPHANUM;
			Keyboard.source = CurNetwork.key;
			break;
		case ACTION_OPTIONS_DHCP:
			CurNetwork.dhcp++;
			if(CurNetwork.dhcp > 1)
			{
				CurNetwork.dhcp = 0;
			}
			break;
		case ACTION_OPTIONS_IP:
			Keyboard.enabled = 1;
			Keyboard.inputLen = 14;
			Keyboard.type = OSK_IP;
			Keyboard.source = CurNetwork.ip;
			break;
		case ACTION_OPTIONS_NETMASK:
			Keyboard.enabled = 1;
			Keyboard.inputLen = 14;
			Keyboard.type = OSK_NUMERIC;
			Keyboard.source = CurNetwork.netmask;
			break;
		case ACTION_OPTIONS_BACK:
			CurrentMenu = MenuMain;
			SelectedItem = menuSwitchItem(CurrentMenu, 0);
			break;

		default:
			break;
	}
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

void menuLoadAll()
{
	MenuMain = menuCreateNew(MenuMain, 0, "NETWORK SETTINGS", ACTION_OPTIONS);
	MenuMain = menuCreateNew(MenuMain, 1, "CONNECT/DISCONNECT", ACTION_CONNECT);
	MenuMain = menuCreateNew(MenuMain, 2, "EXIT", ACTION_QUIT);

	MenuOptions = menuCreateNew(MenuOptions, 0, "MODE", ACTION_OPTIONS_MODE);
	MenuOptions = menuCreateNew(MenuOptions, 1, "ESSID", ACTION_OPTIONS_ESSID);
	MenuOptions = menuCreateNew(MenuOptions, 2, "ENCRYPTION", ACTION_OPTIONS_ENCRYPTION);
	MenuOptions = menuCreateNew(MenuOptions, 3, "PASSWORD", ACTION_OPTIONS_PASSWORD);
	//MenuOptions = menuCreateNew(MenuOptions, 4, "DHCP", ACTION_OPTIONS_DHCP);
	//MenuOptions = menuCreateNew(MenuOptions, 5, "IP", ACTION_OPTIONS_IP);
	//MenuOptions = menuCreateNew(MenuOptions, 6, "NETMASK", ACTION_OPTIONS_NETMASK);
	MenuOptions = menuCreateNew(MenuOptions, 4, "", ACTION_NONE);
	MenuOptions = menuCreateNew(MenuOptions, 5, "BACK", ACTION_OPTIONS_BACK);

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
			while(NewItem->Action == ACTION_NONE)
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
		menuAction(SelectedItem);
	}
	if(keystate[SDLK_LCTRL])
	{
		keystate[SDLK_LCTRL] = 0;
		menuAction(SelectedItem);
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
