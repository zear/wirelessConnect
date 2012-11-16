#ifndef _NETWORK_H_
#define _NETWORK_H_

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
	ENC_WPA
} NetEncryption;

typedef enum NetDhcpEnum
{
	DHCP_ENABLED,
	DHCP_DISABLED
} NetDhcp;

typedef struct NetworkStruct
{
	NetStatus status;
	NetMode mode;
	char essid[61];
	NetEncryption encryption;
	char key[61];
	NetDhcp dhcp;
	int ip;
	int netmask;
} Network;

extern Network CurNetwork;

int networkConnect();
int networkDisconnect();

#endif /* _NETWORK_H */
