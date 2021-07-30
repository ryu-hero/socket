#include"Header.h"

int SockInit(void)
{
	WSADATA data;

	if ((WSAStartup(WINSOCK_VERSION, &data)) != 0) {

		printf("WSAStartup %d", WSAGetLastError);
		return -1;
	}
	return 0;
}


void CleanUp(void)
{
	WSACleanup();
}