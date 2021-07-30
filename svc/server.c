#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <errno.h>
#define PORT 22201
#define CONNECT_MAX 3

void  data_rcv(int clisock );

void decrypt(char *str, int len);
int getmaxfd(int server,int *client);
void child_status(int sts);

int main(void) {

	int srvsock;
	int clisock;

	struct sockaddr_in srvaddr;//  sockaddr_in = 構造体の枠
	struct sockaddr_in cliaddr;
	struct sigaction sa;	
	int cliaddrsize = sizeof(int);
	fd_set fds;
	int pid,n;

	memset(&sa,0,sizeof(sa));
	sa.sa_handler = child_status;
	sa.sa_flags = SA_NOCLDSTOP | SA_RESTART;

	sigaction( SIGCHLD, &sa,NULL );

	srvsock = socket(AF_INET, SOCK_STREAM, 0);//サーバ用ファイルスクリプター番号登録

	//  0 = 　 End to  End プロトコル     

	if ((srvsock < 0)) {

		printf("ソケット生成失敗\n");
		return -1;

	}

	memset(&srvaddr, 0, sizeof(srvaddr));

	srvaddr.sin_family = AF_INET;           //  アドレスの種類  AF_INET = I Pv4

	srvaddr.sin_port = htons(PORT);     // ポート(ネットワーク・バイトオーダー ２バイト)

	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // IPアドレス(ネットワーク・バイトオーダー ４バイト)
	// INADDR_ANY = 0.0.0.0 誰でも許可する

	if (bind(srvsock, (struct sockaddr *) &srvaddr, sizeof(srvaddr)) < 0) {
		printf("バインド失敗\n");
		return -1;
	}
	if (listen(srvsock, CONNECT_MAX + 1) < 0){
		printf("接続許可処理失敗\n");
		return -1;
	}
	printf("接続許可処理成功\n");

	while (1) {

		FD_ZERO(&fds);

		FD_SET(srvsock, &fds); 		//  サーバー・ディスクリプタの設定

		n = select( srvsock + 1, &fds, NULL, NULL,NULL); //  クライアント・ディスクリプタの監視
		
		if(n == -1){ 
			if(errno == EINTR){
				continue;
			}
			printf("select error = %d %s\n",errno,strerror(errno));
			return -1;
		}

		if (FD_ISSET(srvsock, &fds)) {  //  サーバー・ディスクリプタの検査

			clisock = accept(srvsock, (struct sockaddr *)&cliaddr,(socklen_t *)&cliaddrsize);

			if (clisock >= 0) {

				if((pid = fork()) == 0){ // 子プロセスの生成
					data_rcv(clisock);
				}
				printf("接続されました\n");

			}
		}
	}
}

void data_rcv(int clisock ){

	char buf[128];
	int recum;  

	while(1){
		memset(buf, 0, sizeof(buf));
		recum = recv(clisock, buf, sizeof(buf), 0);
		if (recum != 0) {
			decrypt(buf, recum);
			printf("受信データ＝%s\n", buf);
		}else {
			if (close(clisock)) {
				printf("コネクション切断失敗\n");
			}
			clisock = 0;
			printf("切断されました\n");
			break;
		}
	}
	exit(1);
}
void decrypt(char *str, int len) {
	unsigned char ch = 0x00;
	int i;
	for (i = 0; i < len; i++) {
		ch = 0x01 << (i % 8);
		str[i] = str[i] ^ ch;
	}
}

void child_status(int sts){
	waitpid(-1,&sts,WNOHANG);	
}

