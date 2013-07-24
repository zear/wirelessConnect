/*
 * callbacks.c
 *
 *  Created on: Dec 3, 2012
 *      Author: johnny
 */

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include "callbacks.h"
#include "config.h"
#include "menu.h"
#include "network.h"
#include "onScreenKeyboard.h"
#include "logic.h"
#include "fileio.h"

void becomesUppercase(char *s)
{
	int i;
	for (i=0;i<strlen(s);i++)
		{
			s[i] = toupper(s[i]);
		}
}

void becomesLowercase(char *s)
{
	int i;
	for (i=0;i<strlen(s);i++)
		{
			s[i] = tolower(s[i]);
		}
}

void actionStub(MenuItem *this)
{
	printf("MENU ITEM: %i IS A STUB. <fixme>\n", this->number);
}

void actOptions()
{
	CurrentMenu = MenuOptions;
	SelectedItem = menuSwitchItem(CurrentMenu, 0);
}

void actConnect()
{
	if(CurNetwork.status == STATUS_OFF)
	{
		CurNetwork.status = STATUS_CONNECTING;
	}
	else
	{
		networkDisconnect();
		CurNetwork.status = STATUS_OFF;
	}
}

void actQuit()
{
	setGameState(STATE_EXIT);
}

void actOptionsMode()
{
	CurNetwork.mode++;
	if(CurNetwork.mode > 2)
	{
		CurNetwork.mode = 0;
	}
}

void actOptionsESSID()
{
	Keyboard.enabled = 1;
	Keyboard.inputLen = 59;
	Keyboard.kbdType = OSK_ALPHANUM;
	Keyboard.textType = OSK_TEXT_VISIBLE;
	Keyboard.source = CurNetwork.essid;
}

void actOptionsEnc()
{
	CurNetwork.encryption++;
	if(CurNetwork.encryption > 3)
	{
		CurNetwork.encryption = 0;
	}
}

void actOptionsPassword()
{
	Keyboard.enabled = 1;
	Keyboard.inputLen = 59;
	Keyboard.kbdType = OSK_ALPHANUM;
	Keyboard.textType = OSK_TEXT_HIDDEN;
	Keyboard.source = CurNetwork.key;
}

void actOptionsDHCP()
{
	CurNetwork.dhcp++;
	if(CurNetwork.dhcp > 1)
	{
		CurNetwork.dhcp = 0;
	}
}

void actOptionsIP()
{
	Keyboard.enabled = 1;
	Keyboard.inputLen = 14;
	Keyboard.kbdType = OSK_IP;
	Keyboard.textType = OSK_TEXT_VISIBLE;
	Keyboard.source = CurNetwork.ip;
}

void actOptionsNETMASK()
{
	Keyboard.enabled = 1;
	Keyboard.inputLen = 14;
	Keyboard.kbdType = OSK_NUMERIC;
	Keyboard.textType = OSK_TEXT_VISIBLE;
	Keyboard.source = CurNetwork.netmask;
}

void actOptionsTogglePasswordVisibility()
{
	config.showKey = !config.showKey;
}

void actNetworkBack()
{
	CurrentMenu = MenuMain;
	SelectedItem = menuSwitchItem(CurrentMenu, 0);
}

void actOptionsBack()
{
	CurrentMenu = MenuNetwork;
	SelectedItem = menuSwitchItem(CurrentMenu, 0);
}

static int buildProfilesMenu()
{
  struct dirent **namelist;
  int n, i=0;

  n = scandir(homeDir, &namelist, 0, alphasort);
  if (n < 0)
      perror("scandir");
  else {
      while (n--) {
      		if (strlen(namelist[n]->d_name)<4)
      			continue;

      		if (!strcmp(namelist[n]->d_name, "defaultNetwork.cfg"))
      			continue;

      		char *s = &namelist[n]->d_name[strlen(namelist[n]->d_name) - 4];

          printf("%s\n", s);
          if (!strcmp(s, ".cfg"))
          	{
          		i++;
          		char buf[128];
          		memset(&buf, '\0', sizeof(buf));
          		strncpy(buf, namelist[n]->d_name,strlen(namelist[n]->d_name) -4);
          		becomesUppercase(buf);

          		MenuNetworks = menuCreateNew(MenuNetworks, i-1, buf, actProfileLoad);
          	}
          free(namelist[n]);
      }
      free(namelist);
  }

  MenuNetworks = menuCreateNew(MenuNetworks, i, "", NULL);
  MenuNetworks = menuCreateNew(MenuNetworks, i+1, "BACK" , actNetworkBack);

  return i;
}

void actNetworks(){
	menuDeleteSingle(MenuNetworks);
	MenuNetworks = NULL;

	if (buildProfilesMenu() < 1)
		{
			menuDeleteSingle(MenuNetworks);
			printf("No saved profiles found.\n");
			return;
		}

	CurrentMenu = MenuNetworks;
	SelectedItem = menuSwitchItem(MenuNetworks, 0);
}

void actNetwork(){
	CurrentMenu = MenuNetwork;
	SelectedItem = menuSwitchItem(MenuNetwork, 0);
}

void actProfileSave(){
	char buf[128];

	snprintf(buf, sizeof(buf), "/%s%s" ,CurNetwork.essid, ".cfg");
	becomesLowercase(buf);

	saveNetworkConfig(homeDir, buf);
}

void actProfileLoad(MenuItem *this){
	char buf[128];
	snprintf(buf, sizeof(buf), "/%s%s", this->caption,".cfg");
	becomesLowercase(buf);

	printf("Loading profile %s.\n", buf);
	loadNetworkConfig(homeDir, buf, &CurNetwork);
}
