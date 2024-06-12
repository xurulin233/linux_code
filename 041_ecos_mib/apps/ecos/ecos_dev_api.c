
#include <net/if.h> //for struct ifreq
#include <errno.h> //for errno
#include "ecos_dev_api.h"

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
            printf("create socket error\n");
            return -1;
        }
    }

    //
    // do socket ioctl, return err no.
    //
    return ioctl(s_socket_fd, cmd, pdata);
}


static bool_t InterfaceSockaddrSet(
    const char  *cszIfName,
    in_addr_t   in_addr,
    int         request
    )
{
    struct ifreq ifr = {{{0}}};
    struct sockaddr_in *pSockAddr = (struct sockaddr_in *)&ifr.ifr_addr;

    strncpy(ifr.ifr_name, cszIfName, IFNAMSIZ);
    pSockAddr->sin_family = AF_INET;
    pSockAddr->sin_addr.s_addr = htonl(in_addr);

    return dev_socket_ioctl(request, &ifr)>=0;
}

bool_t InterfaceIPSet(const char *cszIfName, in_addr_t ulIP)
{
    int i = 0;

    while (++i <= 5)
    {
        if (InterfaceSockaddrSet(cszIfName, ulIP, SIOCSIFADDR))
            break;
        else
            printf("Set IP addr failed, errno:%d\n", errno);
    }
    return (i <= 5);
}