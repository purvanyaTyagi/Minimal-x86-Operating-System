#include "std/stdint.h"
#include "std/stdlib.h"

void _cdecl cstart_(uint16_t bootDrive)
{
    //switch_protected_mode();
    print("welcome to FEMBos!!\r\n");
    switch_protected();
    while(1);
}
 