#ifndef __COMMON_H__
#define __COMMON_H__




#define	TABLE_COUNT(_table_name_)	(sizeof(_table_name_)/sizeof(_table_name_[0]))

#define FUNCTION_WHILE_START while(1) {
#define FUNCTION_WHILE_END }

typedef enum request_type_e
{
    REQUEST_TYPE_ASSEMBLE,
    REQUEST_TYPE_PARSER,
} request_type_e;


typedef enum node_type_e
{
    DATA_UINT8,
    DATA_UINT16,
    DATA_UINT32,
    DATA_UINT64,
    DATA_STRING,
    DATA_IP,
    DATA_IPSTR,
    DATA_MAC,
    DATA_MACSTR,
    DATA_FLOATSTR,
    DATA_STRUCT,
} node_type_e;


typedef enum handle_result_e
{
	HANDLE_SUCCESSED,
	HANDLE_FAILED,
	HANDLE_ID_UNKNOWN,
	HANDLE_VENDOR_INVALID,
} handle_result_e;


#define FUNCTION_PARA                                           \
	request_type_e          msg_request_type,		\
	unsigned short          vendor_elem_id,			\
	node_type_e             vendor_elem_type,		\
	unsigned short	        vendor_elem_len,		\
	unsigned char     *p_vendor_parser_buff,		\
    	unsigned int      vendor_parser_buff_len

typedef handle_result_e (*vendor_handle)(FUNCTION_PARA);

typedef struct config_node_t
{
	unsigned short   node_id;
	char 	         description[64];
	request_type_e   req_type;
	unsigned short	 len;
	node_type_e node_type;
	vendor_handle    handle;
} config_node_t;



#endif //__COMMON_H__
