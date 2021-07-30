#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>

#define STS_OFF 0
#define STS_ON 1

#define PORT 22201
#define CONNECT_MAX 3

int SockInit(void);
void encrypto(char *str, int len);
