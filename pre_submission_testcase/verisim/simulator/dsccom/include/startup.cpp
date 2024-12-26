#include "vpi_user.h"

extern void initSystemC();
PLI_DLLESPEC void (*vlog_startup_routines[])() =
{
    initSystemC,
    0
};