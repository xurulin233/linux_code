#include <stdio.h>
#include <errno.h>
#include <net/route.h>
#include <net/if.h>
#include <net/route.h>
#include <linux/netlink.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<string.h> // for memset
#include "ecos_dev_api.h"
/****************************************************************************
 *  static functions
 ****************************************************************************/

static int s_socket_fd = INVALID_SOCKET;

int dev_socket_ioctl(int cmd, void* pdata)
{
    //
    // create socket when first time, return error when failed.
    //
    if (INVALID_SOCKET == s_socket_fd)
    {
        if (INVALID_SOCKET == (s_socket_fd=socket(AF_INET, SOCK_DGRAM, 0)))
        {
            printf( "create socket error\n");
            return -1;
        }
    }

    //
    // do socket ioctl, return err no.
    //
    return ioctl(s_socket_fd, cmd, pdata);
}

int dev_file_ioctl(char *sz_name, int cmd, void* pdata)
{
    int dev_fd, error;

    //
    // open dev, do ioctl, close dev, return.
    //
    if ((dev_fd=open(sz_name, O_RDWR)) < 0)
        return -1;

    if ((error=ioctl(dev_fd, cmd, pdata)) < 0)
        return -1;

    close(dev_fd);
    return error;
}

/****************************************************************************
 *  ecos create low socket.
 ****************************************************************************/

int ecos_create_kernel_socket(unsigned short port)
{
    struct sockaddr_nl addr;
    int sock = -1;

    // 
    // create kernel netlink socket fd with port NETLINK_EBI_SYSLOG.
    // 
    if ((sock=socket(AF_NETLINK, SOCK_RAW, port)) < 0)
    {
        perror("create kernel netlink socket");
        return -1;
    }

    // 
    // set netlink addr info, PF_NETLINK, (nl_pid)0, (nl_groups)1.
    // 
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid    = 0;
    addr.nl_groups = 1;

    // 
    // bind netlink socket with addr above.
    // 
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind kernel netlink socket");
        close(sock);
        return -1;
    }

    return sock;
}
