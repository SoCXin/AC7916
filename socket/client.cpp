#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
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
	if (arg < 3)
	{
		return -1;
	}

	int port = atoi(args[2]);
	int st = socket(AF_INET, SOCK_STREAM, 0);//初始化socket

	struct sockaddr_in addr; // 定义一个IP地址的结构
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;// 设置结构地址类型为TCP/IP地址
	addr.sin_port = htons(port); // 制定一个端口号：8080，htons：将short类型从host字节类型转到net字节类型
	// 将字符类型的IP地址转化为int，赋给addr结构
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_addr.s_addr = inet_addr(args[1]);

	//
	if (connect(st, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("connect failed %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	pthread_t thrd1, thrd2;

	pthread_create(&thrd1, NULL, recvsocket, &st);
	pthread_create(&thrd2, NULL, sendsocket, &st);
	pthread_join(thrd1, NULL);
	pthread_join(thrd2, NULL);

	close(st);
	return EXIT_SUCCESS;
}
