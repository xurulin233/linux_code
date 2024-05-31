//server.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handing(char* buf);

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;

    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];
    if(argc != 2)
    {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handing("bind() error");
    if(listen(serv_sock, 5) == -1)
        error_handing("listen() error");

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads); //将服务端套接字注册入fd_set,即添加了服务器端套接字为监视对象
    fd_max = serv_sock;

    while(1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        //无限循环调用select 监视可读事件
        if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1)
        {
            perror("select error");break;
        }
        if(fd_num == 0)
            continue;

        for(i = 0; i < fd_max + 1; i++)
        {
            if(FD_ISSET(i, &cpy_reads))
            {
                /*发生状态变化时,首先验证服务器端套接字中是否有变化.
                ①若是服务端套接字变化，接受连接请求。
                ②若是新客户端连接，注册与客户端连接的套接字文件描述符.
                */
                if(i == serv_sock)
                {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads);
                    if(fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    printf("connected client: %d \n", clnt_sock);
                }
                else    
                {
                    str_len = read(i, buf, BUF_SIZE);
                    if(str_len == 0)    //读取数据完毕关闭套接字
                    {
                        FD_CLR(i, &reads);//从reads中删除相关信息
                        close(i);
                        printf("closed client: %d \n", i);
                    }
                    else
                    {
                        write(i, buf, str_len);//执行回声服务  即echo
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}

void error_handing(char* buf)
{
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}

