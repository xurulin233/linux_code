#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "ecos_config_file.h"

#define CF_MAX_BUF      20480

#define CF_ENTER        "\r\n"                  // key word: enter.
#define CF_VER_KEY      "ecos_version"          // key word: version.

#define CF_DEF_KEY      "key"                   // key word: key.
#define CF_DEF_MIB      "mib"                   // key word: mib.
#define CF_DEF_TYPE     "type"                  // key word: type.
#define CF_DEF_VAL      "val"                   // key word: val.
#define CF_DEF_RANGE    "range"                 // key word: range.
#define CF_DEF_INDEX    "index"                 // key word: index.
#define CF_DEF_COMMENT  "comment"               // key word: comment.


static FILE *s_config_file_fd;                  // file fd of config file.
static char s_read_line[CF_MAX_BUF];            // data buffer when read line.
static char s_content_buf[CF_MAX_BUF];          // data buffer when read content.
static char s_pdata_buf[CF_MAX_BUF];            // data buffer return to other api.
static ecos_config_file_t s_line_info;          // one file line info.


static struct
{
    ecos_config_file_type_e type;
    const char              *key;
} s_key_list[] = 
{
    { ECOS_CF_U32,      "u32"    },             // val: u32.
    { ECOS_CF_IP,       "ip"     },             // val: ip.
    { ECOS_CF_MAC,      "mac"    },             // val: mac.
    { ECOS_CF_STRING,   "string" },             // val: string.
    { ECOS_CF_HEX,      "hex"    },             // val: hex.
};

#define LINE_INFO           ( 0)                // file line has info.
#define LINE_EOF            (-1)                // file seek end.
#define LINE_NO_INFO        (-2)                // file line has no valid info.



static int read_content(char *content, char **ppbuf)
{
    char *pos;

    // 
    // find string first ".
    // 
    if (NULL == (pos=strstr(*ppbuf, "\"")))
        return LINE_NO_INFO;
    pos = pos + 1;

    while (*pos != '\0')
    {
        // 
        // find string escape character ".
        // 
        if ((*pos == '\\') && (*(pos+1) == '\"'))
        {
            *content = '\"';
            pos += 2;
        }
        // 
        // find string last ".
        // 
        else if (*pos == '\"')
        {
            pos += 1;
            goto content_end;
        }
        // 
        // common character, no escape.
        // 
        else
        {
            *content = *pos;
            pos += 1;
        }

        content++;
    }
    return LINE_NO_INFO;

content_end:
    // 
    // return content string.
    // 
    *content++ = '\0';
    *ppbuf = pos;
    return LINE_INFO;
}

static void read_type_val(ecos_config_file_t *info, char *content)
{
    switch (info->type)
    {
    // 
    // these are type: digit.
    // 
    case ECOS_CF_U32:
        sscanf(content, "%u", (unsigned int *)info->pdata);
        info->len = sizeof(unsigned int);
        break;

    // 
    // these are type: ip.
    // 
    case ECOS_CF_IP:
        sscanf(content, "%hhu.%hhu.%hhu.%hhu", 
            ((unsigned char *)info->pdata) + 0, 
            ((unsigned char *)info->pdata) + 1, 
            ((unsigned char *)info->pdata) + 2, 
            ((unsigned char *)info->pdata) + 3
            );
        info->len = sizeof(unsigned int);
        break;

    // 
    // these are type: string.
    // 
    case ECOS_CF_STRING:
        strcpy((char *)info->pdata, content);
        info->len = strlen(content) + 1;
        break;

    // 
    // these are type: hex.
    // 
    case ECOS_CF_MAC:
    case ECOS_CF_HEX:
        while (*content)
        {
            if (*content == ' ')
            {
                content++;
                continue;
            }
            sscanf(content, "%hhx", ((unsigned char *)info->pdata) + info->len);
            info->len += 1;

            while (*content)
            {
                if (*content == ' ')
                    break;
                content++;
            }
        }
        break;

    // 
    // do not support, trace error.
    // 
    default:
        printf("%s(0x%08x) type error.\n", info->key, info->mib);
        return;
    }
}

static int file_get_line(void)
{
    int index;
    char *pos, *buf;

    // 
    // set position.
    // 
    pos = buf = s_read_line;

    // 
    // set info data ptr.
    // 
    s_line_info.pdata = s_pdata_buf;

    // 
    // file seek end.
    // 
    if (NULL == fgets(s_read_line, sizeof(s_read_line), s_config_file_fd))
        return LINE_EOF;

    // 
    // fill ecos_config_file_t key mib type, like:
    // <key "oem name" mib "0x60b30000" type "string">
    // 
    if (NULL != (pos=strstr(buf, "<"CF_DEF_KEY" ")))
    {
        // 
        // read content: key.
        // 
        buf = pos + 1;
        if (LINE_NO_INFO == read_content(s_content_buf, &buf))
            return LINE_NO_INFO;       
        strcpy(s_line_info.key, s_content_buf);

        // 
        // read content: mib.
        // 
        if (NULL == (pos=strstr(buf, CF_DEF_MIB)))
            return LINE_NO_INFO;
        if (LINE_NO_INFO == read_content(s_content_buf, &buf))
            return LINE_NO_INFO;        
        sscanf(s_content_buf, "%x", &s_line_info.mib);

        // 
        // read content: type.
        // 
        if (NULL == (pos=strstr(buf, CF_DEF_TYPE)))
            return LINE_NO_INFO;
        if (LINE_NO_INFO == read_content(s_content_buf, &buf))
            return LINE_NO_INFO;
        for (index=0; index<sizeof(s_key_list)/sizeof(s_key_list[0]); index++)
        {
            if (0 == strcmp(s_key_list[index].key, s_content_buf))
                s_line_info.type = s_key_list[index].type;
        }

        return LINE_NO_INFO;
    }

    // 
    // fill ecos_config_file_t val index, like:
    // <val "wireless3" index "0x21" comment "card 2 vap 3">
    // 
    if (NULL != (pos=strstr(buf, "<"CF_DEF_VAL" ")))
    {       
        // 
        // read content: val.
        // 
        buf = pos + 1;
        if (LINE_NO_INFO == read_content(s_content_buf, &buf))
            return LINE_NO_INFO;
        read_type_val(&s_line_info, s_content_buf);

        // 
        // read content: index.
        // 
        if (NULL == (pos=strstr(buf, CF_DEF_INDEX)))
            return LINE_INFO;
        if (LINE_NO_INFO == read_content(s_content_buf, &buf))
            return LINE_NO_INFO;
        sscanf(s_content_buf, "%hhx", &s_line_info.index);

        return LINE_INFO;
    }

    return LINE_NO_INFO;
}

/****************************************************************************
 * extern functions
 ****************************************************************************/

void ecos_config_file_fprintf(const char *format, ...)
{
    va_list args;

    // 
    // like printf define.
    // 
    va_start(args, format);
    vfprintf(s_config_file_fd, format, args);
    va_end(args);
}

void ecos_config_file_save_key(ecos_config_file_t* config_db)
{
    // 
    // save config file key.
    // 
    if (config_db != NULL)
    {
        ecos_config_file_fprintf("<"CF_DEF_KEY" \"%s\"", config_db->key);
        ecos_config_file_fprintf(" "CF_DEF_MIB" \"0x%08x\"", config_db->mib);
        ecos_config_file_fprintf(" "CF_DEF_TYPE" \"%s\"", s_key_list[config_db->type].key);
        ecos_config_file_fprintf(">"CF_ENTER);
    }
    else
    {
        ecos_config_file_fprintf("</"CF_DEF_KEY">"CF_ENTER CF_ENTER);
    }
}

void ecos_config_file_save_val(ecos_config_file_t* config_db)
{
    unsigned int i;
    
    // 
    // 1. save config file val.
    // 
    ecos_config_file_fprintf("\t<"CF_DEF_VAL" \"");
    switch (config_db->type)
    {
    case ECOS_CF_U32:
        ecos_config_file_fprintf("%u", *(unsigned int *)config_db->pdata);
        break;

    case ECOS_CF_IP:
#ifdef _BIG_ENDIAN_
        ecos_config_file_fprintf("%hhu.%hhu.%hhu.%hhu", 
            ((unsigned char *)config_db->pdata)[0], 
            ((unsigned char *)config_db->pdata)[1], 
            ((unsigned char *)config_db->pdata)[2], 
            ((unsigned char *)config_db->pdata)[3]
            );
#else
        ecos_config_file_fprintf("%hhu.%hhu.%hhu.%hhu", 
            ((unsigned char *)config_db->pdata)[3], 
            ((unsigned char *)config_db->pdata)[2], 
            ((unsigned char *)config_db->pdata)[1], 
            ((unsigned char *)config_db->pdata)[0]
            );
#endif
        break;

    case ECOS_CF_STRING:
        ecos_config_file_fprintf("%s", (char *)config_db->pdata);
        break;

    // 
    // mac use hex, set len to 6.
    // 
    case ECOS_CF_MAC:
        config_db->len = 6;
    case ECOS_CF_HEX:
        for (i=0; i<config_db->len; i++)
            ecos_config_file_fprintf("%02x ", ((unsigned char *)config_db->pdata)[i]);
        break;

    default:
        printf("%s(0x%08x) type error.\n", config_db->key, config_db->mib);
        return;
    }
    ecos_config_file_fprintf("\"");

    // 
    // 2. save config file index.
    // 
    if (ECOS_CONFIG_FILE_NO_INDEX != config_db->index)
    {
        ecos_config_file_fprintf(" "CF_DEF_INDEX" \"0x%02x\"", config_db->index);
    }

    // 
    // 3. save config file comment.
    // 
    if (config_db->comment[0] != '\0')
    {
        ecos_config_file_fprintf(" "CF_DEF_COMMENT" \"%s\"", config_db->comment);
    }

    // 
    // 4. save config file enter.
    // 
    ecos_config_file_fprintf(">"CF_ENTER);
}

ecos_config_file_t* ecos_config_file_load_data(void)
{
    int result;
    
    // 
    // free value info.
    // 
    s_line_info.index = 0;
    s_line_info.len   = 0;

    // 
    // get one file line.
    // 
    result = file_get_line();

    // 
    // file find eof.
    // 
    if (result == LINE_EOF)
        return ECOS_CONFIG_FILE_EOF;

    // 
    // file find some value info.
    // 
    if (result == LINE_INFO)
        return &s_line_info;

    // 
    // can not find valid value data.
    // 
    return NULL;
    
}

/****************************************************************************
 * save / check version
 ****************************************************************************/

void ecos_config_file_save_version(int flag)
{
    if (flag != ECOS_CONFIG_FILE_VERSION_HEAD)
    {
        ecos_config_file_fprintf("</"CF_VER_KEY">"CF_ENTER CF_ENTER);
        return;
    }

    ecos_config_file_fprintf("<!-- config data base -->"CF_ENTER);
    ecos_config_file_fprintf("<!-- note: make sure you should not modify index valude -->"CF_ENTER);
    ecos_config_file_fprintf("<!--       if you need add character \", please set escape code \\\" -->"CF_ENTER CF_ENTER);

    ecos_config_file_fprintf("<"CF_VER_KEY" %s>"CF_ENTER CF_ENTER, ECOS_VERSION_STRING);
}

int ecos_config_file_check_version(const char *path)
{
    char sz_version[64], sz_finish[64];
    int  flag_version=0, flag_finish=0;
    FILE *config_file_fd;

    // 
    // fopen config_file_fd.
    // 
    if (NULL == (config_file_fd=fopen(path, "r")))
        return ECOS_CONFIG_FILE_VERSION_ERROR;

    // 
    // make version and finish string.
    // 
    sprintf(sz_version, "<"CF_VER_KEY" %s>", ECOS_VERSION_STRING);
    sprintf(sz_finish , "</"CF_VER_KEY">");

    // 
    // read version and finish string.
    // 
    while (NULL != fgets(s_read_line, sizeof(s_read_line), config_file_fd))
    {
        // 
        // check file whether have string sz_version.
        // 
        if (0 == strncmp(s_read_line, sz_version, strlen(sz_version)))
        {
            flag_version = 1;
            continue;
        }

        // 
        // check file whether have string sz_finish.
        // 
        if (0 == strncmp(s_read_line, sz_finish, strlen(sz_finish)))
        {
            flag_finish = 1;
            continue;
        }
    }

    // 
    // fclose config_file_fd.
    // 
    fclose(config_file_fd);

    // 
    // return check result.
    // 
    if ((flag_version==1) && (flag_finish==1))
        return ECOS_CONFIG_FILE_VERSION_OK;
    return ECOS_CONFIG_FILE_VERSION_ERROR;
}

/****************************************************************************
 * open / close / getfd
 ****************************************************************************/

int ecos_config_file_open(const char *path, const char *mode)
{
    // 
    // fopen config file.
    // 
    s_config_file_fd = fopen(path, mode);

    return (s_config_file_fd==NULL) ? -1 : 0;
}

void ecos_config_file_close(void)
{
    // 
    // fclose config file when needed.
    // 
    if (s_config_file_fd != NULL)
    {
        fclose(s_config_file_fd);
        sync();
        s_config_file_fd = NULL;
    }
}

/****************************************************************************
 * init / free
 ****************************************************************************/

int ecos_config_file_init(void)
{
    s_config_file_fd = NULL;
    
    return 0;
}

void ecos_config_file_free(void)
{
    ecos_config_file_close();
}
