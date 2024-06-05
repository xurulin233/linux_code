/****************************************************************************
 *  root parser functions.
 *  compatible old configserver version, we will modify later.
 ****************************************************************************/
#include <stdio.h>
#include "ecos_mib_reg.h"
#include "ecos_oem_mib_parser.h"
#include "ecos_addr_rule.h"
#include "ecos_mib.h"

int g_mib_comm_fd = INVALID_SOCKET;


static int parser_mib_root(
    ExchangeHdr_t       *pReqHdr,
    ExchangeHdr_t       *pRespHdr,
    struct sockaddr_in  client_addr
    )
{
    uint8_t *pRespEnd;
    uint8_t *node_real_addr;
    ecos_mib_shash_reg_t *mib_node;

    // 
    // find mib node.
    // 
    mib_node = ecos_mib_find(pReqHdr->ulMIB);

    // 
    // mib node does not registered.
    //

	if (mib_node == NULL)
    {
        pRespHdr->uchResult = CONF_RESULT_MIB_UNKNOWN;
        return 0;
    }

    // 
    // set response end, node real addr.
    // 
    pRespEnd = (uint8_t *)(pRespHdr + 1);
    node_real_addr = mib_node->rule_fn(mib_node->base_addr, mib_node->base_size, pReqHdr->uchIndex);

    //
    // response error when index out of range.
    //
    if (node_real_addr == ECOS_ADDR_ERROR)
    {
        printf("REAL ADDR CONF_RESULT_INDEX_OUT_OF_RANGE!\n");
        pRespHdr->uchResult = CONF_RESULT_INDEX_OUT_OF_RANGE;
        return 0;
    }

    // 
    // client port can not paerser other super request.
    // 
    if ( (pReqHdr->uchRequest != CONF_REQUEST_FIELD_INFO)
      && (pReqHdr->uchRequest != CONF_REQUEST_READ      )
      && (pReqHdr->uchRequest != CONF_REQUEST_WRITE     )
      && (pReqHdr->uchRequest != CONF_REQUEST_REMOVE    )
      && (pReqHdr->uchRequest != CONF_REQUEST_PERMISSION)
        )
    {
        pRespHdr->uchResult = CONF_RESULT_REQUEST_DENY;
        return 0;
    }


    // 
    // check parser function NULL.
    // 
    if (mib_node->parser_fn == NULL)
    {
        pRespHdr->uchResult = CONF_RESULT_REQUEST_NOT_HANDLE;
        return 0;
    }

    // 
    // parser mib node functions.
    // 

		pRespHdr->uchResult = mib_node->parser_fn(
            pReqHdr->uchRequest, 
            pReqHdr->ulMIB, 
            pReqHdr->uchIndex, 
            pReqHdr->usDataLength, 
            node_real_addr, 
            mib_node->base_size, 
            (uint8_t*)(pReqHdr+1), 
            (uint8_t **)&pRespEnd
            );
    // 
    // set resp data len, return data len.
    // 
    pRespHdr->usDataLength = pRespEnd - (uint8_t*)(pRespHdr+1);

    return (int) pRespHdr->usDataLength;
}


/****************************************************************************
 *  init / free.
 ****************************************************************************/

bool_t ecos_mib_parser_fd_read(fd_set* fset)
{
    // 
    // check fd whether to me.
    // 
    if (!FD_ISSET(g_mib_comm_fd, fset))
        return FALSE;

    // 
    // recv request, and parser packet, and send response.
    // 
    ECosMibCommServerExchange(parser_mib_root);

    return TRUE;
}

int ecos_mib_parser_client_init(void)
{
    // 
    // register mib parser root function.
    // compatible old version lib.
    // 
    if ((g_mib_comm_fd=ECosMibCommServerRegister()) < 0)
    {
        printf("register socket failed\n");
        return -1;
    }

    return 0;
}




void ecos_mib_parser_client_free(void)
{
    // 
    // unregister mib comm socket fd.
    // 
    if (FALSE == ECosMibCommServerUnregister())
    {
        printf("unregister socket failed.\n");
    }

    // 
    // free g_mib_comm_fd.
    // 
    g_mib_comm_fd = INVALID_SOCKET;
}
