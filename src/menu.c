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
#include "callbacks.h"

MenuContainer *CurrentMenu = NULL;
MenuItem *SelectedItem = NULL;

MenuContainer *MenuMain = NULL;
MenuContainer *MenuOptions = NULL;
MenuContainer *MenuProfile = NULL;
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
	MenuMain = menuCreateNew(MenuMain, 0, "NETWORK SETTINGS", actProfile);
	MenuMain = menuCreateNew(MenuMain, 1, "CONNECT/DISCONNECT", actConnect);
	MenuMain = menuCreateNew(MenuMain, 2, "EXIT", actQuit);

	MenuProfile = menuCreateNew(MenuProfile, 0, "PROFILE SETTINGS", actOptions);
	MenuProfile = menuCreateNew(MenuProfile, 1, "LOAD PROFILE", actProfiles );
	MenuProfile = menuCreateNew(MenuProfile, 2, "SAVE PROFILE", actionStub );

	MenuProfile = menuCreateNew(MenuProfile, 3, "", NULL );
	MenuProfile = menuCreateNew(MenuProfile, 4, "BACK", actProfileBack );


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

void menuDrawSingle(MenuContainer *Container, int number, int x, int y, int sz)
{
	MenuItem *CurrentItem = Container->Menu;

	if(number < 0 || number > Container->size)
	{
		fprintf(stderr, "ERROR (menuDraw): No such menu: %d\n", number);
		return;
	}

	int min = SelectedItem->number-3;

	while(CurrentItem != NULL)
	{
		if (CurrentItem->number < min && sz>7)
			{
				CurrentItem = CurrentItem->Next;
				continue;
			}

		if(CurrentItem->number == number)
		{
				int p;
				if (min >= 0 && sz>7)
					{
						p = CurrentItem->number - min;
						printf("%i\n", sz);
					}
				else
					{
						p = CurrentItem->number;
					}

				p *= 15;
				p += y;

			drawText(CurrentItem->caption, x, p, &FontUnifont);

			if(CurrentItem == SelectedItem)
			{
				drawText(">", 60, p, &FontUnifont);
				drawText("<", SCREEN_WIDTH - 60, p, &FontUnifont);
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
		menuDrawSingle(CurrentMenu, i, x, y, CurrentMenu->size);
	}
}
