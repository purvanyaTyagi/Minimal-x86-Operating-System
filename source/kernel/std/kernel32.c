#include "stdint.h"
#include "fesh.c"

void __attribute__((cdecl)) start(uint16_t bootDrive){
    main_fesh();
    while (1);
}
