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

#include "menu.h"
#include "network.h"
#include "onScreenKeyboard.h"
#include "logic.h"
#include "fileio.h"

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
	Keyboard.type = OSK_ALPHANUM;
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
	Keyboard.type = OSK_ALPHANUM;
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
	Keyboard.type = OSK_IP;
	Keyboard.source = CurNetwork.ip;
}

void actOptionsNETMASK()
{
	Keyboard.enabled = 1;
	Keyboard.inputLen = 14;
	Keyboard.type = OSK_NUMERIC;
	Keyboard.source = CurNetwork.netmask;
}

void actProfileBack()
{
	CurrentMenu = MenuMain;
	SelectedItem = menuSwitchItem(CurrentMenu, 0);
}

void actOptionsBack()
{
	CurrentMenu = MenuProfile;
	SelectedItem = menuSwitchItem(CurrentMenu, 0);
}

static void buildProfilesMenu()
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

      		char *s = &namelist[n]->d_name[strlen(namelist[n]->d_name) - 4];

          printf("%s\n", s);
          if (!strcmp(s, ".cfg"))
          	{
          		i++;
          		char buf[128];
          		strncpy(buf, namelist[n]->d_name,strlen(namelist[n]->d_name) -4);
          		becomesUppercase(buf);

          		MenuProfiles = menuCreateNew(MenuProfiles, i-1, buf, actionStub);
          	}
          free(namelist[n]);
      }
      free(namelist);
  }

  MenuProfiles = menuCreateNew(MenuProfiles, i, "", NULL);
  MenuProfiles = menuCreateNew(MenuProfiles, i+1, "BACK" , actProfileBack);

}

void actProfiles(){
	menuDeleteSingle(MenuProfiles);
	MenuProfiles = NULL;

	buildProfilesMenu();

	CurrentMenu = MenuProfiles;
	SelectedItem = menuSwitchItem(MenuProfiles, 0);
}

void actProfile(){
	CurrentMenu = MenuProfile;
	SelectedItem = menuSwitchItem(MenuProfile, 0);
}
