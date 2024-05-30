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

#define	MAX_EXCHANGE_SIZE		(100 * 1024)// Note: Max Size Is 64k(2 byte)


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

int ECosMibCommServerRegister(void);
bool_t ECosMibCommServerUnregister(void);
bool_t ECosMibCommServerExchange(void);
int ECosMibCommClientRegister(ECosMibCommPort_t usCliPort);
bool_t ECosMibCommClientUnregister(void);
ExchangeResult_t ConfigExchange(u8 *pData,u32 ulSndSize);
#endif// __ECOS_MIB_COMM_H__

