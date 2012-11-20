#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char cmdSetIfaceUp[30];
char cmdSetIfaceDown[30];
char cmdSetMode[120];
char cmdSetEssid[120];
char cmdSetWepKey[120];
char cmdSetWpaPassphrase[120];
char cmdSetWpaSup[120];
char cmdSetDhcp[30];

int networkConnect()
{
	switch(CurNetwork.mode)
	{
		case NMODE_MANAGED:
		sprintf(cmdSetMode, "iwconfig %s mode managed", CurNetwork.interface);
		break;
		case NMODE_ADHOC:
		sprintf(cmdSetMode, "iwconfig %s mode ad-hoc", CurNetwork.interface);
		break;
		case NMODE_MASTER:
		sprintf(cmdSetMode, "iwconfig %s mode master", CurNetwork.interface);
		break;

		default:
		break;
	}
	sprintf(cmdSetMode, "ifconfig %s up", CurNetwork.interface);
	sprintf(cmdSetIfaceDown, "ifconfig %s down", CurNetwork.interface);
	sprintf(cmdSetEssid, "iwconfig %s essid \"%s\"", CurNetwork.interface, CurNetwork.essid);
	sprintf(cmdSetDhcp, "udhcpc -i %s -n", CurNetwork.interface);
	switch(CurNetwork.encryption)
	{
		case ENC_WEP:
			sprintf(cmdSetWepKey, "iwconfig %s key s:%s", CurNetwork.interface, CurNetwork.key);
			break;
		case ENC_WEP_NUM:
			sprintf(cmdSetWepKey, "iwconfig %s key %s", CurNetwork.interface, CurNetwork.key);
			break;
		case ENC_WPA:
			sprintf(cmdSetWpaPassphrase, "wpa_passphrase \"%s\" \"%s\" > /tmp/wpa.conf", CurNetwork.essid, CurNetwork.key);
			sprintf(cmdSetWpaSup, "wpa_supplicant -B -Dwext -i%s -c/tmp/wpa.conf", CurNetwork.interface);
			break;

		default:
			break;
	}

	system(cmdSetIfaceDown);
	if(system(cmdSetMode))
	{
		return 1;
	}
	if(system(cmdSetEssid))
	{
		return 1;
	}
	switch(CurNetwork.encryption)
	{
		case ENC_NONE:
			if(system(cmdSetIfaceUp))
			{
				return 1;
			}
			sleep(1);
			break;
		case ENC_WEP:
		case ENC_WEP_NUM:
			if(system(cmdSetWepKey))
			{
				return 1;
			}
			if(system(cmdSetIfaceUp))
			{
				return 1;
			}
			sleep(1);
			break;
		case ENC_WPA:
			if(system(cmdSetIfaceUp))
			{
				return 1;
			}
			sleep(1);
			if(system(cmdSetWpaPassphrase))
			{
				return 1;
			}
			if(system(cmdSetWpaSup))
			{
				return 1;
			}
			sleep(1);
			break;

		default:
			break;
	}
	if(system(cmdSetDhcp))
	{
		return 1;
	}

	return 0;
}

int networkDisconnect()
{
	system(cmdSetIfaceDown);
}
