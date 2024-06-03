#include "ecos_type.h"

/****************************************************************************
 *
 * DESCRIPTION:
 *  basic ioctl functions.
 *
 ****************************************************************************/

int dev_socket_ioctl(int cmd, void* pdata);
int dev_file_ioctl(char *sz_name, int cmd, void* pdata);

/****************************************************************************
 * FUNCTION:
 *  ecos_create_kernel_socket
 *
 * DESCRIPTION:
 *  create low socket.
 *
 * PARAMETERS:
 *  port -- socket port.
 *
 * RETURNS:
 *  socket fd.
 *
 ****************************************************************************/

int ecos_create_kernel_socket(unsigned short port);
