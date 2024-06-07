#ifndef __ECOS_MIB_COMM_H__
#define __ECOS_MIB_COMM_H__

#include "ecos_type.h"

#define	ECOS_DEF_SERVER_IP			"127.0.0.1"

typedef enum ECosMibCommPort_t
{
	// Server
	ECOS_MIB_COMM_UDP_CFG_SERVER = 8800,

	ECOS_MIB_COMM_RANDOM = ~0,
} ECosMibCommPort_t;

#define EXCHANGE_HDR_LEN		sizeof(ExchangeHdr_t)

#define	MAX_EXCHANGE_SIZE		(100 * 1024)// Note: Max Size Is 64k(2 byte)
typedef	struct
{
    uint8_t     ubl4CardIndex   :4,
                ubh4VAPIndex    :4;
} IndexByteField_t;

typedef struct ExchangeHdr_t
{
	uint32_t	ulMIB;
	uint16_t	usDataLength;// exclude header len
	union
	{
		IndexByteField_t	index;
		uint8_t				uchIndex;
	};
	uint8_t		uchRequest;
	uint8_t		uchResult;
	uint8_t		uchReserved[3];
} ExchangeHdr_t;

typedef	enum ExchangeRequestType_t
{
	// Exchange Request Type
	CONF_REQUEST_FIELD_INFO = 0,
	CONF_REQUEST_READ,
	CONF_REQUEST_WRITE,
	CONF_REQUEST_REMOVE,
	CONF_REQUEST_HIT,
	CONF_REQUEST_PERMISSION,
} ExchangeRequestType_t;

typedef	enum ExchangeResult_t
{
	// Exchange Result
	CONF_RESULT_OK = 0,
	CONF_RESULT_IPC_ERROR,
	CONF_RESULT_VERSION_MISMATCH,
	CONF_RESULT_MIB_UNKNOWN,
	CONF_RESULT_REQUEST_DENY,
	CONF_RESULT_REQUEST_NOT_HANDLE,
	CONF_RESULT_INDEX_OUT_OF_RANGE,
	CONF_RESULT_DATA_TOO_SHORT,
	CONF_RESULT_DATA_TOO_LONG,
	CONF_RESULT_DATA_INVALID,
	CONF_RESULT_UNKNOWN,
	CONF_RESULT_INFO = 0x80
} ExchangeResult_t;

typedef	enum ExchangeFieldType_t
{
	CONF_FIELD_UNKNOWN = 0,
	CONF_FIELD_INTEGER,
	CONF_FIELD_ENUM,
	CONF_FIELD_BITMAP,	// same as enum, only different is reuqired to bit test.
	CONF_FIELD_STRING,
	CONF_FIELD_ARRAY,
	CONF_FIELD_STRUCT
} ExchangeFieldType_t;

typedef	struct ExchangeFieldInfoInteger_t
{
	uint32_t	ulMin;
	uint32_t	ulMax;
	uint32_t	ulStep;
} ExchangeFieldInfoInteger_t;

typedef	struct ExchangeEnumInfo_t
{
	uint32_t	ulValue;
	char		*szNameOrOffset;
} ExchangeEnumInfo_t;

typedef	struct ExchangeFieldInfoEnum_t
{
	uint32_t			ulCount;
	ExchangeEnumInfo_t	EnumInfos[1];
} ExchangeFieldInfoEnum_t;

typedef	struct ExchangeFieldInfoString_t
{
	uint32_t	ulLengthMin;	// include '\0'
	uint32_t	ulLengthMax;	// include '\0'
} ExchangeFieldInfoString_t;

typedef	struct ExchangeFieldInfoArray_t
{
	uint32_t	ulUnitCountMin;
	uint32_t	ulUnitCountMax;
	uint32_t	ulUnitSize;
} ExchangeFieldInfoArray_t;

typedef enum ExchangeAccessType_t	
{
	CONF_ACCESS_UNUSABLE = 0,
	CONF_ACCESS_READONLY,
	CONF_ACCESS_WRITEONLY,
	CONF_ACCESS_READWRITE,
	CONF_ACCESS_LAST
} ExchangeAccessType_t;

typedef	struct ExchangeFieldInfo_t
{
	uint32_t	ulFieldType;// ExchangeFieldType_t
	union
	{
		ExchangeFieldInfoInteger_t	InfoInteger;
		ExchangeFieldInfoEnum_t		InfoEnum;
		ExchangeFieldInfoString_t	InfoString;
		ExchangeFieldInfoArray_t	InfoArray;
	};
} ExchangeFieldInfo_t;

typedef	int (*ServerExchangeHook_t)(
	ExchangeHdr_t *pReqHdr, 
	ExchangeHdr_t *pRespHdr, 
	struct sockaddr_in client_addr
	);

///////////////////////////////////////////////////////////////////////////////
// extern functions
///////////////////////////////////////////////////////////////////////////////
int ECosMibCommServerRegister(void);
//int ECosMibCommSyslogRegister(ECosMibCommPort_t usCliPort);
bool_t ECosMibCommServerUnregister(void);
bool_t ECosMibCommServerExchange(ServerExchangeHook_t ServerExchangeHook);

int ECosMibCommClientRegister(ECosMibCommPort_t usCliPort);
int ECosMibCommClientRegisterExtServer(char *szIP, u16 usPort);
bool_t ECosMibCommClientUnregister(void);

///////////////////////////////////////////////////////////////////////////////
// compatible with the old version
// these functions used for client side
///////////////////////////////////////////////////////////////////////////////

ExchangeResult_t ConfigExchange(
	ExchangeRequestType_t	Request,
	uint8_t		uchIndex,
	uint32_t	ulMIB,
	uint8_t		*pData,
	uint32_t	ulSndSize,
	uint32_t	ulRcvSize
	);

int	GetLastExchangeResponse(
	uint32_t ulMIB, 
	uint8_t *pData,
	uint32_t ulOffset,
	uint32_t ulSize
	);

ExchangeResult_t ConfigRequestFieldInfo(
	uint8_t		uchIndex,
	uint32_t	ulMIB,
	uint8_t		*pData,
	uint32_t	ulSize
	);


#define	CONF_READ_STRING(INDEX, MIB, STRING)	\
	ConfigReadString(INDEX, MIB, STRING, sizeof(STRING))

#define	CONF_READ_STRING0(MIB, STRING)			\
	CONF_READ_STRING(0, MIB, STRING)

static __inline ExchangeResult_t ConfigReadString(
	uint8_t		uchIndex,
	uint32_t	ulMIB,
	char		*szString,
	uint32_t	ulSize
	)
{
	return ConfigExchange(
		CONF_REQUEST_READ,
		uchIndex,
		ulMIB,
		(uint8_t*)szString,
		0,
		ulSize
		);
}


static __inline ExchangeResult_t ConfigWriteString(
	uint8_t		uchIndex,
	uint32_t	ulMIB,
	const char	*cszString
	)
{
	return ConfigExchange(
		CONF_REQUEST_WRITE,
		uchIndex,
		ulMIB,
		(uint8_t*)cszString,
		strlen(cszString)+1,
		0
		);
}


#endif //__ECOS_MIB_COMM_H__