#ifndef __ECOS_IP_V4_H__
#define __ECOS_IP_V4_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ecos_type.h"
#include "ecos_mib_comm.h"

///////////////////////////////////////////////////////////////////////////////
//	UDP
///////////////////////////////////////////////////////////////////////////////
extern SOCKET ECosUdpCreate(u16 SrvPort);
extern i32    ECosUdpClose(SOCKET UDP_SrvSocket);
extern i32    ECosUdpRecv(SOCKET UDP_SrvSocket, struct sockaddr_in *UDP_CliAddr,  i8 *pBuff, u32 BuffSize);
extern i32    ECosUdpSend(SOCKET UDP_SrvSocket, struct sockaddr_in *UDP_SendAddr, i8 *pBuff, i32 BuffLen);
extern void   ECosUdpCreateConnAddr(struct sockaddr_in *UDP_SendAddr, ip_t ConnIp, u16 ConnPort);



#endif //__ECOS_IP_V4_H__
