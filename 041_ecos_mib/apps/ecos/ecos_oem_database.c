#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "ecos_oem_database.h"
#include "ecos_configset.h"

int ecos_oem_database_default(void *arg)
{
    strcpy(g_ConfigSet.szProductName,"FishTank");
    printf("database_default Set szProductName to FishTank\n");
    return 0;
}