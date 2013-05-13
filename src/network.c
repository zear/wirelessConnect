#include "network.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#ifndef  _NO_IFADDRS
#include <ifaddrs.h>
#endif
#include <errno.h>

#include <arpa/inet.h>

char cmdSetIfaceUp[30];
char cmdSetIfaceDown[30];
char cmdSetMode[120];
char cmdSetEssid[120];
char cmdSetWepKey[120];
char cmdSetWpaPassphrase[120];
char cmdSetWpaSup[120];
char cmdSetDhcp[30];

int systemf(const char *fmt, ...)
{
	char __sys[1024];
	int ret_code;
	va_list args;
	va_start(args, fmt);
	//TODO: CODE
		vsnprintf(__sys, 1023, fmt, args);
		ret_code = system(__sys);
	va_end(args);

	return ret_code;
}

void updateIpAddress()
{
#ifdef _NO_IFADDRS
	/*
	 * DEPRECATED CODE.
	 */
  int fd;
  struct ifreq ifr;
  struct sockaddr_in *sock_in;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, CurNetwork.interface, IFNAMSIZ-1);
  sock_in = (struct sockaddr_in *)&ifr.ifr_addr;

  ioctl(fd, SIOCGIFADDR, &ifr);

  strncpy(CurNetwork.ip, inet_ntoa(sock_in->sin_addr), sizeof(CurNetwork.ip));

  close(fd);
#else

  //-----------------------------------------------------------------------
  // TODO: Take care of IPV6, and display that.
  //-----------------------------------------------------------------------

  struct ifaddrs *interfaces = NULL, *itrAddr = NULL;
  void *addrPtr = NULL;
  char buf[INET6_ADDRSTRLEN];

  if (getifaddrs(&interfaces) == 0) //Queries the kernel for interfaces
  	{
  		for(itrAddr = interfaces; itrAddr != NULL; itrAddr = itrAddr->ifa_next) //Iterates through 'em.
  			{
  				if (strcmp(itrAddr->ifa_name, CurNetwork.interface)!=0) //Ignore anything we're not searching for.
  					continue;

  				if (itrAddr->ifa_addr->sa_family == AF_INET) //Is it IPV4 or IPV6?
  					addrPtr = &((struct sockaddr_in *)itrAddr->ifa_addr)->sin_addr;
  				else
  					continue; //We still won't take care of IPV6.
  				//addrPtr = &((struct sockaddr_in6 *)itrAddr->ifa_addr)->sin6_addr; //Not taking care of IPV6 atm

  				inet_ntop(itrAddr->ifa_addr->sa_family, addrPtr, CurNetwork.ip, sizeof(CurNetwork.ip));
  			}
  	}

  freeifaddrs(interfaces);
  CurNetwork.status = STATUS_ON;
#endif
}

int queryInterfaceStatus()
{
#ifdef _NO_IFADDRS
	return loadTemp("/tmp", "/.wiCon.tmp");
#else
  struct ifaddrs *interfaces = NULL, *itrAddr = NULL;
  void *addrPtr = NULL;
  char buf[INET6_ADDRSTRLEN];

  if (getifaddrs(&interfaces) == 0) //Queries the kernel for interfaces
  	{
  		for(itrAddr = interfaces; itrAddr != NULL; itrAddr = itrAddr->ifa_next) //Iterates through 'em.
  			{
  				if (!strcmp(itrAddr->ifa_name, CurNetwork.interface)==0) //Ignore anything we're not searching for.
  					continue;

  				return itrAddr->ifa_flags & IFF_UP;
  			}
  	}

  freeifaddrs(interfaces);
#endif
}

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
	sprintf(cmdSetIfaceUp, "ifconfig %s up", CurNetwork.interface);
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

	systemf("ifconfig %s down", CurNetwork.interface);

#if defined(DRIVER_WEXT)
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
#elif defined(DRIVER_NL80211)
	systemf("killall -9 wpa_supplicant");
	if(systemf("ifconfig %s up", CurNetwork.interface))
	{
		return 1;
	}
	sleep(1);
	switch(CurNetwork.encryption)
	{
		case ENC_NONE:
			break;
		case ENC_WEP:
		case ENC_WEP_NUM:
			systemf("echo -e \"network={\\nssid=\\\"%s\\\"\\nscan_ssid=1\\npriority=5\\nkey_mgmt=NONE\\nwep_key0=\\\"%s\\\"\\nwep_tx_keyidx=0\\n}\" > /tmp/wpa.conf", CurNetwork.essid, CurNetwork.key);
			systemf("wpa_supplicant -B -Dnl80211 -i%s -c/tmp/wpa.conf", CurNetwork.interface);
			sleep(1);
			break;
		case ENC_WPA:
			systemf("wpa_passphrase \"%s\" \"%s\" > /tmp/wpa.conf", CurNetwork.essid, CurNetwork.key);
			systemf("wpa_supplicant -B -Dnl80211 -i%s -c/tmp/wpa.conf", CurNetwork.interface);
			sleep(1);
			break;

		default:
			break;
	}
#endif
	if(systemf("udhcpc -i %s -n", CurNetwork.interface))
	{

            return 1;
	}

	updateIpAddress();

	return 0;
}

int networkDisconnect()
{
	system(cmdSetIfaceDown);
}
