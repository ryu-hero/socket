#include"Header.h"
int main(void) {
	int sts = STS_OFF;
	char buff[256];
	struct sockaddr_in server_addr;
	int sock;


unsigned long ip = inet_addr("192.168.9.36");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = ip;


	while (1) {
		memset(buff, 0, sizeof(buff));
		printf("コマンドを入力：");
		scanf("%s", buff);
		if ((strcmp(buff, "START")) == 0) {	   //ソケット生成してコネクトする。
			if (sts == STS_ON) {
				continue;
			}
			sock = socket(AF_INET, SOCK_STREAM, 0);

			if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr))) {

				printf("コネクション確立失敗\n ");
				return -1;
			}
			sts = STS_ON;
			printf("コネクションが確立されました。\n");
		}
		else if ((strcmp(buff, "END") == 0)) { //ディスコネクトする。
			if (sts == STS_OFF) {
				continue;
			}
			close(sock);
			sts = STS_OFF;

		}
		else if ((strcmp(buff, "TERM") == 0)) {//プログラムを終了する。
			return 0;

		}
		else {								   //データを送信する
			if (sts == STS_OFF) {
				continue;
			}
			encrypto(buff, strlen(buff));
			if (send(sock, buff, strlen(buff), 0) == -1) {

				printf("送信失敗 \n");
				return -1;
			}
		}
	}
}

void encrypto(char *str, int len) {
	unsigned char ch = 0x00;
	int i;
	for (i = 0; i < len; i++) {
		ch = 0x01 << (i % 8);
		str[i] = str[i] ^ ch;
	}
}
