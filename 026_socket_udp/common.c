#include <stdio.h>
#include <string.h>
#include "common.h"


static handle_result_e handle_assemble_printf(FUNCTION_PARA)
{

	unsigned char *p_data=NULL;
	unsigned short len = 0;
	p_data = p_vendor_parser_buff;
	char buff[128];
	len = vendor_parser_buff_len;

	if (p_data == NULL || len <= 0)
	{
		return HANDLE_VENDOR_INVALID;
	}

	if (!(msg_request_type & REQUEST_TYPE_PARSER))
	{
		return HANDLE_VENDOR_INVALID;
	}

	memset(buff,0,sizeof(buff));
	memcpy(buff,p_data, vendor_parser_buff_len);
	printf("handle_assemble_printf vendor_elem_id = %d   %s\n",vendor_elem_id,buff);

	return HANDLE_SUCCESSED;
}



static const config_node_t s_config_nodes[] =
{
    { 
        0x0001, "ap mac address",   REQUEST_TYPE_PARSER,               128, 
	 DATA_STRING, 	handle_assemble_printf
    },

    { 
        0x0002, "ap mac address",   REQUEST_TYPE_PARSER,               128, 
	 DATA_STRING,   handle_assemble_printf
    },

};

handle_result_e parser_config(FUNCTION_PARA)
{
	
	unsigned int count;

	for (count=0; count<TABLE_COUNT(s_config_nodes); count++)
	{
	
		if (s_config_nodes[count].node_id == vendor_elem_id)
		{
	
			handle_result_e ret;
			 if (s_config_nodes[count].handle == NULL)
            		{
                		return HANDLE_SUCCESSED;
            		}


			ret = s_config_nodes[count].handle
                    (
                  	msg_request_type,
                        s_config_nodes[count].node_id,     //配置项对应的vendor_elem_id
			s_config_nodes[count].node_type,   //配置项数据类型
                        s_config_nodes[count].len,         //配置项的最大长度
                        p_vendor_parser_buff,                   //AC下达的配置内容
                        vendor_parser_buff_len                 //待解析内容的长度
                    );
			 return ret;
		}
	}

	return HANDLE_ID_UNKNOWN;
}





