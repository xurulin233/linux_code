#include "ecos_sockv4.h"

#define TCP_MAX_LISTEN		(10)	// max concurrent connect, not max client num
#define CLIENT_PORT_BASE	(10000)
#define CLIENT_PID_MAX		(50000)

///////////////////////////////////////////////////////////////////////////////
//	Common
///////////////////////////////////////////////////////////////////////////////
static int socket_server_create(int iSockType, u16 SrvPort)
{
	SOCKET sock = INVALID_SOCKET;
	struct sockaddr_in addrBind;
	unsigned short client_port;
	int yes = 1;

	if ((sock=socket(AF_INET, iSockType, 0)) < 0)// create
	{
		printf("Error creating socket=%d\n", sock);
		return -1;
	}

	memset(&addrBind, 0, sizeof(addrBind));
	addrBind.sin_port		 = htons(SrvPort);
	addrBind.sin_family 	 = AF_INET;
	addrBind.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if (SrvPort != (u16)ECOS_MIB_COMM_RANDOM)
	{
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));// fast disconnect -> but can re bind
	}
	else
	{
		if (getpid() >= CLIENT_PID_MAX)
		{
			printf("%s(%d) warning: pid(%d)>=%d, client socket may empty.\n", 
				__FUNCTION__, __LINE__, getpid(), CLIENT_PID_MAX);
		}
		
		client_port = getpid()%CLIENT_PID_MAX + CLIENT_PORT_BASE; // range: 1xxxx~49999
		addrBind.sin_port = htons(client_port);
	}
	
	if (bind(sock, (struct sockaddr*)&addrBind, sizeof(addrBind)) < 0)// bind
	{
		printf("Error bind socket\n");
		close(sock);
		return -1;
	}
	
	if (SOCK_STREAM == iSockType)// if tcp server, set max listen
	{
		listen(sock, TCP_MAX_LISTEN);
	}

	return sock;
}

static int socket_client_create(int iSockType, ip_t ServerIp, u16 ServerPort)
{
	SOCKET sock = INVALID_SOCKET;
	struct sockaddr_in addrConnect;
	int yes = 1;

	if ((sock=socket(AF_INET, iSockType, 0)) < 0)// create
	{
		printf("Error creating socket=%d\n", sock);
		return -1;
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));// fast disconnect -> but can re bind

	addrConnect.sin_port		= htons(ServerPort);
	addrConnect.sin_family		= AF_INET;
	addrConnect.sin_addr.s_addr = ServerIp;
	if(connect(sock, (struct sockaddr*)&addrConnect, sizeof(struct sockaddr_in)) < 0)// connect
	{
		printf("Error connect socket\n");
		close(sock);
		return -1;
	}

	return sock;
}

///////////////////////////////////////////////////////////////////////////////
//	UDP
///////////////////////////////////////////////////////////////////////////////
SOCKET ECosUdpCreate(u16 SrvPort)
{
	// create and bind udp port
	
	return socket_server_create(SOCK_DGRAM, SrvPort);
}

i32 ECosUdpClose(SOCKET UDP_SrvSocket)
{
	// close fd what u bind
	
	return close(UDP_SrvSocket);
}
i32 ECosUdpRecv(SOCKET UDP_SrvSocket, struct sockaddr_in *UDP_CliAddr, i8 *pBuff, u32 BuffSize)
{
	// UDP_SrvSocket: fd what u bind
	// UDP_CliAddr: [Output] remote msg addr

	static int addrLen = sizeof(struct sockaddr_in);

	return recvfrom(UDP_SrvSocket, pBuff, BuffSize, 0, (struct sockaddr *)UDP_CliAddr, (socklen_t *)&addrLen);
}

i32 ECosUdpSend(SOCKET UDP_SrvSocket, struct sockaddr_in *UDP_SendAddr, i8 *pBuff, i32 BuffLen)
{
	// UDP_Socket: fd what u bind
	// UDP_SendAddr: remote addr what want to send

	return sendto(UDP_SrvSocket, pBuff, BuffLen, 0, (struct sockaddr *)UDP_SendAddr, sizeof(struct sockaddr_in));
}

void ECosUdpCreateConnAddr(struct sockaddr_in *UDP_SendAddr, ip_t ConnIp, u16 ConnPort)
{
	// make udp remote addr (sockaddr_in)
	// u also can use recvfrom to get a remote addr

	UDP_SendAddr->sin_port		  = htons(ConnPort);
	UDP_SendAddr->sin_family	  = AF_INET;
	UDP_SendAddr->sin_addr.s_addr = htonl(ConnIp);
}
