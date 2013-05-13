#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdio.h>

typedef enum NetStatusEnum
{
	STATUS_OFF,
	STATUS_ON,
	STATUS_CONNECTING,
	STATUS_FAILED
} NetStatus;

typedef enum NetModeEnum
{
	NMODE_MANAGED,
	NMODE_ADHOC,
	NMODE_MASTER
} NetMode;

typedef enum NetEncryptionEnum
{
	ENC_NONE,
	ENC_WEP,
	ENC_WEP_NUM,
	ENC_WPA
} NetEncryption;

typedef enum NetDhcpEnum
{
	DHCP_ENABLED,
	DHCP_DISABLED
} NetDhcp;

typedef struct NetworkStruct
{
	char interface[10];
	NetStatus status;
	NetMode mode;
	char essid[61];
	NetEncryption encryption;
	char key[61];
	NetDhcp dhcp;
	char ip[16];
	char netmask[16];
} Network;

extern Network CurNetwork;

int systemf(const char *fmt, ...);

int networkConnect();
int networkDisconnect();

int queryInterfaceStatus();
void updateIpAddress();

#endif /* _NETWORK_H */
