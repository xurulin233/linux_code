#ifndef __ECOS_TYPE_H__
#define __ECOS_TYPE_H__


///////////////////////////////////////////////////////////////////////////////
// c++ compatible
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif



///////////////////////////////////////////////////////////////////////////////
// Use such the clause to comment unmature code
#define	TODO				0
///////////////////////////////////////////////////////////////////////////////



#define	TRUE		1
#define	FALSE		0
#define MAC_LEN		6


typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;


typedef char				i8;
typedef short				i16;
typedef int				i32;
typedef long long			i64;
typedef u8				mac_t[MAC_LEN];
typedef u32				ip_t;

#ifndef	INVALID_SOCKET
#define	INVALID_SOCKET		-1
#endif


typedef	u32					bool_t;
typedef	u8					MacAddr_t[MAC_LEN];
typedef	void				T_VOID;
typedef void 				VOID;
typedef	u8					T_BOOL8;
typedef	i8					T_CHAR8;
typedef	u8					T_UCHAR8;
typedef	i8					T_INT8;
typedef	u8					T_UINT8;
typedef	i16					T_INT16;
typedef	u16					T_UINT16;
typedef	i32					T_INT32;
typedef	u32					T_UINT32;
typedef	u32					ipaddr;
typedef u32					T_SEMID_t;
typedef u32					IPAddr_t;
typedef u16					Port_t;
typedef	int					SOCKET;


typedef enum MacAddrType_t
{
	MAC_NULL = 0,
	MAC_MULTICAST,
	MAC_BROADCAST,
	MAC_UNICAST
} MacAddrType_t;




///////////////////////////////////////////////////////////////////////////////
// c++ compatible
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif


#endif //__ECOS_TYPE_H__
