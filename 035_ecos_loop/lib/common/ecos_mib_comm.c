#include "ecos_mib_comm.h"
#include "ecos_sockv4.h"

static int s_iSockServer = INVALID_SOCKET;
static int s_iSockClient = INVALID_SOCKET;

static u8 s_Request[MAX_EXCHANGE_SIZE]  = {0};
static u8 s_Response[MAX_EXCHANGE_SIZE] = {0};

static struct sockaddr_in s_client_addr;// client addr
static struct sockaddr_in s_server_addr;// server addr

int ECosMibCommServerRegister(void)
{
	if ((s_iSockServer=ECosUdpCreate(ECOS_MIB_COMM_UDP_CFG_SERVER)) < 0)
	{
		printf("mibcomm server socket create error\n");
		exit(1);
	}

	return s_iSockServer;
}

bool_t ECosMibCommServerUnregister(void)
{
	if (s_iSockServer > 0)
	{
		if (ECosUdpClose(s_iSockServer) < 0)
			return FALSE;
		s_iSockServer = INVALID_SOCKET;
	}
	return TRUE;
}

bool_t ECosMibCommServerExchange(void)
{
	char ecos_response[100] = " form configserver";
	// This Function Must use 'FD_ISSET' first
	// len of request buff include header len
	i32 iRequestBuffLen = ECosUdpRecv(s_iSockServer, &s_client_addr, (i8 *)s_Request, sizeof(s_Request));
    if (iRequestBuffLen >= 0)
    {
        printf("server recv - %s\n",s_Request);
        memcpy(s_Response, s_Request, iRequestBuffLen);
        ECosUdpSend(s_iSockServer, &s_client_addr, (i8 *)s_Response, iRequestBuffLen);
        printf("server send - %s\n",s_Response);
        return TRUE;
    }

	return FALSE;
}



int ECosMibCommClientRegister(ECosMibCommPort_t usCliPort)
{
	
	if ((s_iSockClient=ECosUdpCreate(usCliPort)) < 0)
	{
		printf("mibcomm client socket create error\n");
		exit(1);
	}
	ECosUdpCreateConnAddr(&s_server_addr, INADDR_LOOPBACK, ECOS_MIB_COMM_UDP_CFG_SERVER);
	
	return s_iSockClient;
}

bool_t ECosMibCommClientUnregister(void)
{
	if (s_iSockClient > 0)
	{
		if (ECosUdpClose(s_iSockClient) < 0)
			return FALSE;
		s_iSockClient = INVALID_SOCKET;
		memset(&s_server_addr, 0x00, sizeof(s_server_addr));
	}
	return TRUE;
}


ExchangeResult_t ConfigExchange(u8 *pData,u32 ulSndSize)
{
	u32 ulReturn = 0;

	if (s_iSockClient == INVALID_SOCKET)
	{
		ECosMibCommClientRegister(ECOS_MIB_COMM_RANDOM);
	}


	if (NULL==pData && 0!=ulSndSize)
	{
		return CONF_RESULT_DATA_INVALID;
	}

	if (ulSndSize > 0)
	{
		memcpy((uint8_t*)(s_Request), pData, ulSndSize);
	}

	// send request
	ulReturn = ECosUdpSend(s_iSockClient, &s_server_addr, (i8 *)s_Request, ulSndSize);
	// recv response
	ulReturn = ECosUdpRecv(s_iSockClient, &s_client_addr, (i8 *)s_Response, sizeof(s_Response));

	// Decode the response
    if (ulReturn > 0)
	{
			memcpy(pData, s_Response, ulReturn);
			pData[ulReturn+1] = '\0';

            return CONF_RESULT_OK;
	}
	return CONF_RESULT_UNKNOWN;
}
