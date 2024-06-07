#ifndef	__ECOS_MACRO_H__
#define	__ECOS_MACRO_H__

///////////////////////////////////////////////////////////////////////////////
// c++ compatible
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// macro
///////////////////////////////////////////////////////////////////////////////
#ifndef	FIELD_OFFSET
	#define	FIELD_OFFSET(_struct_name_, _field_name_)	((uint32_t)((unsigned long)&(((_struct_name_ *)0)->_field_name_)))
#else
	#warning "Macro FIELD_OFFSET already defined!!!"
#endif

#define	FIELD_SIZE(_struct_name_, _field_name_)		(sizeof((((_struct_name_ *)0)->_field_name_)))
#define	FIELD_INFO(_struct_name_, _field_name_)		FIELD_OFFSET(_struct_name_, _field_name_), FIELD_SIZE(_struct_name_, _field_name_)
#define	TABLE_COUNT(_table_name_)					(sizeof(_table_name_)/sizeof(_table_name_[0]))

///////////////////////////////////////////////////////////////////////////////
// c++ compatible
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#endif// __ECOS_MACRO_H__