#include <stdio.h>
#include <string.h> //for strstr


#define LINE_INFO           ( 0)                // file line has info.
#define LINE_EOF            (-1)                // file seek end.
#define LINE_NO_INFO        (-2)                // file line has no valid info.

#define CF_MAX_BUF      20480

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
    ecos_config_file_type_e         // ....
                    type;           // type of val.
    void            *pdata;         // ptr of data.
    unsigned int    len;            // len of pdata.
} ecos_config_file_t;

char s_pdata_buf[CF_MAX_BUF];            // data buffer return to other api.
ecos_config_file_t s_line_info;          // one file line info.


int read_content(char *content,char **ppbuf)
{
	char *pos;
	//
    	// find string first ".
    	//
	if (NULL == (pos=strstr(*ppbuf,"\"")))
		return LINE_NO_INFO;
	pos = pos + 1;

	while(*pos != '\0')
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
    	// do not support, trace error.
    	// 
    	default:
        printf("type error.\n");
        return;

    }
	
}



int main()
{
	char content[100];
	char *buf;
	// 
    	// set info data ptr.
    	// 
    	s_line_info.pdata = s_pdata_buf;
	s_line_info.type = ECOS_CF_IP;

	char read_line[100] = "xurulin\"haha\" \"xixi\" \"hehe\" \"192.168.70.182\"";

	char read_ip_line[100] = "  \"192.168.70.182\"";
	char *ipbuf;
	ipbuf = read_ip_line;


	buf = read_line;
	while (LINE_INFO == read_content(content,&buf))
	{
	printf("##%s\n",content);
	}

	read_content(content,&ipbuf);
	printf("##%s\n",content);
	read_type_val(&s_line_info,content);
	printf("%X\n",*(unsigned int *)s_line_info.pdata);

}
