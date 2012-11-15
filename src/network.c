#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int networkConnect()
{
	char cmdSetEssid[120];
	char cmdSetWepKey[120];
	char cmdSetWpaPassphrase[120];

	sprintf(cmdSetEssid, "iwconfig wlan0 essid \"%s\"", CurNetwork.essid);
	sprintf(cmdSetWepKey, "iwconfig wlan0 key s:%s", CurNetwork.key);
	sprintf(cmdSetWpaPassphrase, "wpa_passphrase \"%s\" \"%s\" > /tmp/wpa.conf", CurNetwork.essid, CurNetwork.key);

	system("ifconfig wlan0 down");
	system("iwconfig wlan0 mode managed");
	system(cmdSetEssid);
	switch(CurNetwork.encryption)
	{
		case ENC_NONE:
		system("ifconfig wlan0 up");
		sleep(1);
		break;
		case ENC_WEP:
		system(cmdSetWepKey);
		system("ifconfig wlan0 up");
		sleep(1);
		break;
		case ENC_WPA:
		system("ifconfig wlan0 up");
		sleep(1);
		system(cmdSetWpaPassphrase);
		system("wpa_supplicant -B -Dwext -iwlan0 -c/tmp/wpa.conf");
		sleep(1);
		break;

		default:
		break;
	}
	//system("dhcpcd wlan0");
	if(system("udhcpc -i wlan0 -n"))
	{
		return 1;
	}

	return 0;
}

int networkDisconnect()
{
	system("ifconfig wlan0 down");
}
