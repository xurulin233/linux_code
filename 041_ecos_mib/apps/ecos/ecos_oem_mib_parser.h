#ifndef __ECOS_OEM_MIB_PARSER_H__
#define __ECOS_OEM_MIB_PARSER_H__

#include "ecos_mib_comm.h"



#define ECOS_PARSER                     \
    __attribute__((unused))

#define MIB_PARSER_PARAMETERS           \
    ExchangeRequestType_t   Request,    \
    uint32_t    ulMIB,                  \
    uint8_t     uchIndex,               \
    uint16_t    usRequestLength,        \
    uint8_t     *pCfgField,             \
    uint32_t    ulCfgSize,              \
    uint8_t     *pReq,                  \
    uint8_t     **ppResp

#define MIB_PARSER_NET_PARAMETERS       \
    Request, ulMIB, uchIndex, usRequestLength, pCfgField, ulCfgSize, pReq, ppResp


#define ecos_exchange_result_t      ExchangeResult_t
#define ecos_exchange_req_type_t    ExchangeRequestType_t
#define ECOS_MIB_PARSER_PARAMETERS  MIB_PARSER_PARAMETERS


extern int g_mib_comm_fd;
bool_t ecos_mib_parser_fd_read(fd_set* fset);

int  ecos_mib_parser_client_init(void);
void ecos_mib_parser_client_free(void);

#endif // __ECOS_OEM_MIB_PARSER_H__
