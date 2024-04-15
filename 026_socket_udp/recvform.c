#include<stdio.h>
#include<string.h>
#include<sys/socket.h>//socket
#include<sys/types.h>
#include<netinet/in.h>//struct sockaddr_in
#include<arpa/inet.h>//inet_pton
#include <fcntl.h> // for open
#include <unistd.h> // for close
int main()
{
	//创建一个UDP套接字
	int sockfd = socket(AF_INET,SOCK_DGRAM,0);
	
	//如果收数据 尽量bind
	struct sockaddr_in my_addr;
	bzero(&my_addr,sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(8000);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockfd,(struct sockaddr *)&my_addr, sizeof(my_addr));
	
	while(1)
	{
		char buf[128]="";
		struct sockaddr_in from;
		socklen_t from_len = sizeof(from);
		//带阻塞
		int len = recvfrom(sockfd, buf, sizeof(buf),0, \
		(struct sockaddr *)&from, &from_len);
		
		
		//分析发送者的信息（IP地址 port）
		unsigned short port = ntohs(from.sin_port);
		char ip[16]="";
		//将from中32位整形IP转换成 点分十进制数串
		inet_ntop(AF_INET,&from.sin_addr.s_addr, ip, 16);
		printf("消息来之%s:%hu\n",ip,port);
		
		printf("len = %d\n",len);
		printf("buf=%s\n",buf);
	}
	close(sockfd);
	return 0;
}

