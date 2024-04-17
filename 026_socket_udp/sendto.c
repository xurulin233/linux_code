#include<stdio.h>
#include<string.h>
#include<sys/socket.h>//socket
#include<sys/types.h>
#include<netinet/in.h>//struct sockaddr_in
#include<arpa/inet.h>//inet_pton
#include <fcntl.h> // for open
#include <unistd.h> // for close


typedef struct message_node
{
	unsigned short node_id;
	unsigned short node_len;
	char buf[128];
}message_node;

int main()
{
	//1、创建一个udp套接字
	int sockfd = socket(AF_INET, SOCK_DGRAM,0);
	
	//2、发送数据
	//定义一个IPv4 目的地址结构 192.168.0.110 8080
	struct sockaddr_in dst_addr;
	//清空结构体
	//memset(&dst_addr,0,sizeof(dst_addr));
	bzero(&dst_addr,sizeof(dst_addr));
	dst_addr.sin_family = AF_INET;//协议
	//将主机字节序转换成网络字节序
	dst_addr.sin_port = htons(8000);//端口
	//将字符串"192.168.0.110" 转换成32位整形数据 赋值IP地址
	inet_pton(AF_INET,"127.0.0.1", &dst_addr.sin_addr.s_addr);
	
	message_node message;
	message.node_id = htons(0x0001);
	message.node_len = htons(strlen("hehe"));
	strcpy(message.buf,"hehe");

	sendto(sockfd,&message,sizeof(message_node),0, \
	(struct sockaddr *)&dst_addr , sizeof(dst_addr) );
	
	//3、关闭套接字
	close(sockfd);
	return 0;
}

