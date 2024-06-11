#include <stdio.h>
#include <string.h> //for strstr


#define LINE_INFO           ( 0)                // file line has info.
#define LINE_EOF            (-1)                // file seek end.
#define LINE_NO_INFO        (-2)                // file line has no valid info.



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

int main()
{
	char content[100];
	char *buf;
	char read_line[100] = "xurulin\"haha\" \"xixi\" \"hehe\" \"192.168.70.182\"";
	buf = read_line;
	while (LINE_INFO == read_content(content,&buf))
	{
	printf("##%s\n",content);
	}

}
