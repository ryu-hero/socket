#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
int g_num = 100;

main()
{
	int i,j=10;
	int pid,sts;
	g_num++;

	printf("forkでの子プロセスの起動\n\n");

	if((i=fork()) == 0){

		/* execute only in CHILD process */
		printf("-------------------------------\n");
		printf("CHILD process Start \n");
		printf("g_num = %d \n",g_num);

		printf("関数forkの値（子供）: %d\n",i);

		printf("（子供の）プロセスＩＤ : %d\n",getpid());
		printf("%d:子供のプロセス \n", ++j);
		printf("%d:子供のプロセス \n", ++j);
		printf("%d:子供のプロセス \n", ++j);
		printf("%d:子供のプロセス \n", ++j);

		sleep(10);
		exit (55);
	} else { 

		/* execute only in PARENT process */
		printf("-------------------------------\n");
		printf("PARENT  process Start \n");
		printf("g_num = %d \n",g_num);

		printf("関数forkの値（親） : %d\n",i);

		printf("（親の）プロセスＩＤ : %d\n",getpid());
		printf("%d:親のプロセス \n", ++j);	
		printf("%d:親のプロセス \n", ++j);
		printf("%d:親のプロセス \n", ++j);
		printf("%d:親のプロセス \n", ++j);
		printf("waitpid start\n");

		pid = waitpid(-1,&sts,0);

		if (WIFEXITED(sts)) {
			printf("exited, status=%d\n", WEXITSTATUS(sts));
		} else if (WIFSIGNALED(sts)) {
			printf("killed by signal %d\n", WTERMSIG(sts));
		} else if (WIFSTOPPED(sts)) {
			printf("stopped by signal %d\n", WSTOPSIG(sts));
		} else if (WIFCONTINUED(sts)) {
			printf("continued\n");
		}		
		printf("waitpid end pid = %d sts = %d\n",pid,WEXITSTATUS(sts));

		sleep(30);


	}
}

