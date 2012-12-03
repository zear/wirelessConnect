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

      		if (!strcmp(namelist[n]->d_name, "config.cfg"))
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

          		MenuProfiles = menuCreateNew(MenuProfiles, i-1, buf, actProfileLoad);
          	}
          free(namelist[n]);
      }
      free(namelist);
  }

  MenuProfiles = menuCreateNew(MenuProfiles, i, "", NULL);
  MenuProfiles = menuCreateNew(MenuProfiles, i+1, "BACK" , actProfileBack);

  return i;
}

void actProfiles(){
	menuDeleteSingle(MenuProfiles);
	MenuProfiles = NULL;

	if (buildProfilesMenu() < 1)
		{
			menuDeleteSingle(MenuProfiles);
			printf("No saved profiles found.\n");
			return;
		}

	CurrentMenu = MenuProfiles;
	SelectedItem = menuSwitchItem(MenuProfiles, 0);
}

void actProfile(){
	CurrentMenu = MenuProfile;
	SelectedItem = menuSwitchItem(MenuProfile, 0);
}

void actProfileSave(){
	char buf[128];

	snprintf(buf, sizeof(buf), "/%s%s" ,CurNetwork.essid, ".cfg");
	becomesLowercase(buf);

	saveConfig(homeDir, buf);
}

void actProfileLoad(MenuItem *this){
	char buf[128];
	snprintf(buf, sizeof(buf), "/%s%s", this->caption,".cfg");
	becomesLowercase(buf);

	printf("Loading profile %s.\n", buf);
	loadConfig(homeDir, buf, &CurNetwork);
}
