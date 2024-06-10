#ifndef __ECOS_OEM_DATABASE_H__
#define __ECOS_OEM_DATABASE_H__

#include "ecos_mib_reg.h"

#define ECOS_CONFIG_FILE            "/home/s5pv210/test1219/linux_code/041_ecos_mib/ecos.cfg"
void ecos_load_common(ecos_config_file_t *config_file, void *arg);
int ecos_config_file_load(void *arg);

#endif // __ECOS_OEM_DATABASE_H__