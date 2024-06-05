#include "ecos_mib_comm.h"
#include "ecos_sockv4.h"

// ############################################################################
// Note: ecos_mib_comm is only design for internal apps communication,
//       so only use ipv4 protocol.

    /****************************************************************
     *                        Data    Format                        *
     *                                                              *
     *           1     2     3     4     5     6     7     8        *
     *      +-----+-----+-----+-----+-----+-----+-----+-----+       *
     *      |          MIB          | Data Len  |index| Req |       *
     *      +-----+-----+-----+-----+-----+-----+-----+-----+       *
     *      | Ret |  for alignment  |       Data Area               *
     *      +-----+-----+-----+-----+-----+-----+-----+-----+       *
     *            Data Area ... ... ... ... ... ... ... ...         *
     *      +-----+-----+-----+-----+-----+-----+-----+-----+       *
     *            Data Area ... ... ... ... ... ... ... ... ~       *
     *      +-----+-----+-----+-----+-----+-----+-----+-----+       *
     *                                                              *
     *      Example: (Edit)                                         *
     *      01 10 03 00 00 00 00 00     00 33 DD 07 00 00 1A 00     *
     *      00 25 00 13 00 01 FF 00     00 00 0C 00 00 06 1F 00     *
     *      01 00 B0 ED D1 E9 10                                    *
     ****************************************************************/

static int s_iSockServer = INVALID_SOCKET;
static int s_iSockClient = INVALID_SOCKET;

static u8 s_Request[MAX_EXCHANGE_SIZE]  = {0};
static u8 s_Response[MAX_EXCHANGE_SIZE] = {0};
static ExchangeHdr_t *s_pReqHdr  = (ExchangeHdr_t *)s_Request;
static ExchangeHdr_t *s_pRespHdr = (ExchangeHdr_t *)s_Response;

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

bool_t ECosMibCommServerExchange(ServerExchangeHook_t ServerExchangeHook)
{
	// This Function Must use 'FD_ISSET' first
	// len of request buff include header len
	i32 iRequestBuffLen = ECosUdpRecv(s_iSockServer, &s_client_addr, (i8 *)s_Request, sizeof(s_Request));

	if ( (iRequestBuffLen==(typeof(iRequestBuffLen))(sizeof(ExchangeHdr_t)+s_pReqHdr->usDataLength))
	  && (NULL!=ServerExchangeHook) )
	{
		// build the response header
		s_pRespHdr->ulMIB        = s_pReqHdr->ulMIB;
		s_pRespHdr->usDataLength = 0;
		s_pRespHdr->uchIndex     = s_pReqHdr->uchIndex;
		s_pRespHdr->uchRequest   = s_pReqHdr->uchRequest;
		s_pRespHdr->uchResult    = CONF_RESULT_IPC_ERROR;

		// Parser Packet
		// len of request buff exclude header len
		i32	iResponseBuffLen = ServerExchangeHook(s_pReqHdr, s_pRespHdr, s_client_addr);
		if (iResponseBuffLen >= 0)
		{
			ECosUdpSend(s_iSockServer, &s_client_addr, (i8 *)s_Response, iResponseBuffLen+sizeof(ExchangeHdr_t));
			return TRUE;
		}
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

ExchangeResult_t ConfigExchange(
	ExchangeRequestType_t	Request,
	uint8_t		uchIndex,
	uint32_t	ulMIB,
	uint8_t		*pData,
	uint32_t	ulSndSize,
	uint32_t	ulRcvSize
	)
{
	u32 ulReturn = 0;

	if (s_iSockClient == INVALID_SOCKET)
	{
		ECosMibCommClientRegister(ECOS_MIB_COMM_RANDOM);
	}

	if (MAX_EXCHANGE_SIZE < ulSndSize || MAX_EXCHANGE_SIZE < ulRcvSize)
	{
		return CONF_RESULT_IPC_ERROR;
	}
	if (NULL==pData && 0!=(ulSndSize|ulRcvSize))
	{
		printf("error data buffer not exist! send(%d) receive(%d) 0x%08x\n",
			ulSndSize, ulRcvSize, ulMIB);
		return CONF_RESULT_DATA_INVALID;
	}

	// build the request header
	s_pReqHdr->ulMIB        = ulMIB;
	s_pReqHdr->usDataLength = (u16)ulSndSize;
	s_pReqHdr->uchIndex     = uchIndex;
	s_pReqHdr->uchRequest   = Request;
	s_pReqHdr->uchResult    = CONF_RESULT_IPC_ERROR;
	if (ulSndSize > 0)
	{
		memcpy((uint8_t*)(s_pReqHdr+1), pData, ulSndSize);
	}

	// send request
	ulReturn = ECosUdpSend(s_iSockClient, &s_server_addr, (i8 *)s_Request, ulSndSize+sizeof(ExchangeHdr_t));
	// recv response
	ulReturn = ECosUdpRecv(s_iSockClient, &s_client_addr, (i8 *)s_Response, sizeof(s_Response));
	
	// Decode the response
	if (s_pRespHdr->ulMIB != s_pReqHdr->ulMIB)
	{
		printf("response MIB mismatch, ReqMIB=%x, RespMIB=%x\n",
			s_pReqHdr->ulMIB, s_pRespHdr->ulMIB);
		s_pRespHdr->uchResult = CONF_RESULT_UNKNOWN;
		return s_pRespHdr->uchResult;
	}
	if (s_pRespHdr->uchResult == CONF_RESULT_OK)
	{
		if ((s_pRespHdr->usDataLength>0) && (pData!=NULL))
		{
			u8	*pResult = (u8 *)(s_pRespHdr+1);
			
			if (ulRcvSize < s_pRespHdr->usDataLength)
			{
				memcpy(pData, pResult, ulRcvSize);
				printf("receive buffer %d too short, required %d! 0x%08x\n",
						ulRcvSize, s_pRespHdr->usDataLength, ulMIB);
				return CONF_RESULT_DATA_TOO_SHORT;
			}

			memcpy(pData, pResult, s_pRespHdr->usDataLength);
			if (ulRcvSize>s_pRespHdr->usDataLength)
			{
				pData[s_pRespHdr->usDataLength] = '\0';
			}
		}
	}
	
	return s_pRespHdr->uchResult;
}

int	GetLastExchangeResponse(
	uint32_t ulMIB, 
	uint8_t *pData,
	uint32_t ulOffset,
	uint32_t ulSize
	)
{
	uint8_t	*pResult = (uint8_t*)(s_pRespHdr+1) + ulOffset;
	int	iRemainderSize = s_pRespHdr->usDataLength - ulOffset;

	if (CONF_RESULT_OK!=s_pRespHdr->uchResult || 0==s_pRespHdr->usDataLength)
	{
		return 0;
	}

	if (ulMIB!=s_pRespHdr->ulMIB)
	{
		return -CONF_RESULT_MIB_UNKNOWN;
	}

	if (iRemainderSize<0)
	{
		iRemainderSize = 0;
	}

	if (0!=(ulSize&iRemainderSize) && (uint32_t)iRemainderSize>ulSize)
	{
		iRemainderSize = ulSize;
	}

	if (NULL!=pData && 0!=iRemainderSize)
	{
		memcpy(pData, pResult, iRemainderSize);
	}
	return iRemainderSize;
}

ExchangeResult_t ConfigRequestFieldInfo(
	uint8_t		uchIndex,
	uint32_t	ulMIB,
	uint8_t		*pData,
	uint32_t	ulSize
	)
{
	ExchangeFieldInfo_t *pFieldInfo = (ExchangeFieldInfo_t *)pData;

	ExchangeResult_t ExchangeResult = ConfigExchange(
		CONF_REQUEST_FIELD_INFO,
		uchIndex,
		ulMIB,
		pData,
		0,
		ulSize
		);

	if (CONF_RESULT_OK!=ExchangeResult)
	{
		//memset(pulData, 0 , ulSize);
		return ExchangeResult;
	}
	if (CONF_FIELD_ENUM == pFieldInfo->ulFieldType)
	{
		ExchangeEnumInfo_t *pEnumInfo = pFieldInfo->InfoEnum.EnumInfos;
		uint32_t ulIndex;

		for (ulIndex=0; ulIndex<pFieldInfo->InfoEnum.ulCount; ulIndex++)
		{
			pEnumInfo[ulIndex].szNameOrOffset += (size_t)pData;
		}
	}
	return CONF_RESULT_OK;
}
