#ifndef __ECOS_CONFIG_FILE_H__
#define __ECOS_CONFIG_FILE_H__

#include <stdlib.h>
#include <stdio.h>

#define ECOS_EXT_NONE               0x00000000      // ext_flag default flag.
#define ECOS_EXT_DB_SAVE            0x00000001      // fap need to save/load config.
#define ECOS_EXT_DB_TAP_SAVE        0x00000002      // thinap need to save/load config.

#define ECOS_VERSION_X          "3"         // ecos sdk version major.
#define ECOS_VERSION_Y          "3"         // ecos sdk version minor.
#define ECOS_VERSION_Z          "1"         // ecos sdk version release.

#define ECOS_VERSION            ( atoi(ECOS_VERSION_X) * 100 \
                                + atoi(ECOS_VERSION_Y) * 10  \
                                + atoi(ECOS_VERSION_Z) )    // version int.

#define ECOS_VERSION_STRING     ECOS_VERSION_X    \
                                "."ECOS_VERSION_Y \
                                "."ECOS_VERSION_Z           // version string.

#define ECOS_COMPILE_TIME       __DATE__" - "__TIME__       // compile time.


#define ECOS_CONFIG_FILE_NO_INDEX   (unsigned char)(~0)

typedef enum ecos_config_file_type_e
{
    ECOS_CF_U32,                    // unsigned int.
    ECOS_CF_IP,                     // unsigned int(ip).
    ECOS_CF_MAC,                    // char [6].
    ECOS_CF_STRING,                 // string.
    ECOS_CF_HEX,                    // hex format.
} ecos_config_file_type_e;

typedef struct ecos_config_file_t
{
    char            key[64];        // key of config db, note overflow.
    unsigned int    mib;            // mib id.
    ecos_config_file_type_e         // ....
                    type;           // type of val.
    unsigned char   index;          // index define like configserver.
    char            comment[64];    // comment of data value, like: route lan wireless.
    void            *pdata;         // ptr of data.
    unsigned int    len;            // len of pdata.
} ecos_config_file_t;



void ecos_config_file_fprintf(const char *format, ...);

void ecos_config_file_save_key(ecos_config_file_t* config_db);
void ecos_config_file_save_val(ecos_config_file_t* config_db);

#define ECOS_CONFIG_FILE_EOF    (ecos_config_file_t *)(~0)

ecos_config_file_t* ecos_config_file_load_data(void);


#define ECOS_CONFIG_FILE_VERSION_HEAD   0
#define ECOS_CONFIG_FILE_VERSION_TAIL   1

void ecos_config_file_save_version(int flag);

#define ECOS_CONFIG_FILE_VERSION_OK     (0)
#define ECOS_CONFIG_FILE_VERSION_ERROR  (1)
#define ECOS_CONFIG_FILE_LOAD_SUCCESS   (0)
#define ECOS_CONFIG_FILE_LOAD_FAILED    (1)
int ecos_config_file_check_version(const char *path);
int ecos_config_file_open(const char *path, const char *mode);
void ecos_config_file_close(void);
int ecos_config_file_init(void);
void ecos_config_file_free(void);

#endif //__ECOS_CONFIG_FILE_H__