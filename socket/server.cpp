
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void* recvsocket(void *arg)
{
	int st = *(int *)arg;
	char s[1024];

	while (1)
	{
		memset(s, 0, sizeof(s));
		int rc = recv(st, s, sizeof(s), 0);
		if (rc <= 0)
			break;
		printf("%s\n", s);
	}
	return NULL;
}

void* sendsocket(void *arg)
{
	int st = *(int *)arg;
	char s[1024];

	while (1)
	{
		memset(s, 0, sizeof(s));
		read(STDIN_FILENO, s, sizeof(s));
		send(st, s, strlen(s), 0);
	}

	return NULL;
}


int main(int arg, char *args[])
{
	if (arg < 2)
	{
		return -1;
	}
	int port = atoi(args[1]);
	int st = socket(AF_INET, SOCK_STREAM, 0); //初始化socket

	int on = 1;
	if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		printf("setsockopt failed %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	struct sockaddr_in addr; // 定义一个IP地址结构
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET; // 将addr结构的属性定位为TCP/IP地址
	addr.sin_port = htons(port); // 将本地字节顺序转化为网络字节顺序
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY代表这个server上所有的地址

	// 将ip与server程序绑定
	if (bind(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		printf("bind failed %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	// server端开始listen
	if (listen(st, 20) == -1)
	{
		printf("listen failed %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	//printf("listen success\n");
	int client_st = 0;
	struct sockaddr_in client_addr; // 表示client端的IP地址

	pthread_t thrd1, thrd2;
	while (1)
	{
		memset(&client_addr, 0, sizeof(client_addr));
		socklen_t len = sizeof(client_addr);
		client_st = accept(st, (struct sockaddr*) &client_addr, &len);
		if (client_st == -1)
		{
			printf("accept failed %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
		printf("accept by %s\n", inet_ntoa(client_addr.sin_addr));
		pthread_create(&thrd1, NULL, recvsocket, &client_st);
		pthread_create(&thrd2, NULL, sendsocket, &client_st);
	}
	close(st);
	return 0;
}