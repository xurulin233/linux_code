#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "ecos_sockv4.h"
#include "ecos_macro.h"
#include "ecos_mib_reg.h"
#include "ecos_oem_root.h"

#define MAIN_LOOP_START     do {
#define MAIN_LOOP_REPEAT    } while (1);


void oem_main_init(void)
{    

    
    // 
    // init mib register functions.
    // 
    if (ecos_mib_reg_init() != 0)
    {
        printf("ecos_mib_reg_init error!\n");
    }

    // 
    // init config file functions.
    // 
    if (ecos_config_file_init() != 0)
    {
       printf("ecos_config_file_init error!\n");
    }



    // 
    // init ecos vendor functions.
    // 
    if (ecos_oem_root_init() != 0)
    {
        printf("ecos_oem_functions_init error!\n");
    }


}


void oem_main_loop(void)
{

    typedef bool_t (*fd_parser)(fd_set* fset);

    int i, fd_max = 0;          // fd_max  : used for select multi-fd.
    fd_set fset;                // fset    : fill all fd to this node.
    struct timeval timeout;     // timeout : max select timeout.

    struct                      // struct for select table.
    {
        int         fd;         // file device what need to selected.
        fd_parser   parser;     // file device parser function.
    } s_file_device_table[] =
    {
        { g_mib_comm_fd,     ecos_mib_parser_fd_read       },  // fd what rev cli/webs/snmp/...
        //{ g_button_check_fd, ecos_product_dev_button_check },  // read button check netlink.
        /* ----------------------------------------------- */  // you can add more dev to here.
        /* ----------------------------------------------- */  // you can add more dev to here.
    };

    // 
    // get max fd value, don't modify this, see 'note' above.
    // 
  
	for (i=0; i<TABLE_COUNT(s_file_device_table); i++)
    {
        if (s_file_device_table[i].fd < 0) continue;

        if (fd_max < s_file_device_table[i].fd)
            fd_max = s_file_device_table[i].fd;
    }

    MAIN_LOOP_START // start main loop.
    // 
    // set select interval is 10ms, free fset.
    // 
    timeout.tv_sec  = 0;
    timeout.tv_usec = 10000;
    FD_ZERO(&fset);

    // 
    // set all fds to fd_set, for select.
    // 
    for (i=0; i<TABLE_COUNT(s_file_device_table); i++)
    {
        if (s_file_device_table[i].fd < 0) continue;
        
        FD_SET(s_file_device_table[i].fd, &fset);
    }

    // 
    // select fd table, and deal fd tables.
    // 
    if (select(fd_max+1, &fset, NULL, NULL, &timeout) >= 0)
    {
        for (i=0; i<TABLE_COUNT(s_file_device_table); i++)
        {
            if (s_file_device_table[i].fd < 0) continue;

            if (TRUE == s_file_device_table[i].parser(&fset))
                break; // when return TRUE, select finish.
        }
    }

    MAIN_LOOP_REPEAT // repeat main loop to start.

}


void oem_main_free(void)
{
    ECosMibCommServerUnregister();
    g_mib_comm_fd = INVALID_SOCKET;
}


int main(int argc, char *argv[])
{
    // 
    // main: init, loop, free.
    // 
    oem_main_init();
    oem_main_loop();
    oem_main_free();  

}




