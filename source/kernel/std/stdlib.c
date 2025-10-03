#include "stdint.h"
#include "stdlib.h"
#include "disk_read.h"

void _cdecl x86_Video_WriteCharTeletype(char c, uint8_t page);
void _cdecl x86_diskread_function(uint16_t lba, uint16_t load_addr_segment, uint16_t load_addr_offset);
uint16_t _cdecl x86_readbyte_function(uint16_t addr_segment, uint16_t addr_offset);
void _cdecl x86_protected_mode_jump();
struct BootSector g_BootSector = {512, 1, 2, 0x0E0, 9}; 
uint16_t g_RootDirectoryEnd;

void putc(char c)   
{
    x86_Video_WriteCharTeletype(c, 0);
}

void print(const char* str)
{
    while(*str)
    {
        putc(*str);
        str++;
    }
}
void load_lba(uint16_t lba){
    x86_diskread_function(lba, load_addr_segment, load_addr_offset); 
    print("\r\n");
    print("loaded segment into memory at 0x00500\r\n");
}


void load_root(){
    unsigned short i = 0;
    uint16_t lba_root = g_BootSector.SectorsPerFat * g_BootSector.FatCount + g_BootSector.ReservedSectors;
    uint16_t size_root = 32 * g_BootSector.DirEntryCount;
    uint16_t sectors = (size_root)/(g_BootSector.BytesPerSector);
    if(size_root % g_BootSector.BytesPerSector > 0)
        sectors++;
    g_RootDirectoryEnd = lba_root + sectors;
    while(i < sectors){
        x86_diskread_function(lba_root + i, load_addr_segment, load_addr_offset + (512*i));
        i++;
    }
    print("loaded root directory into memory address 0x00500\r\n");
    i = 0;
}

uint16_t read_byte(uint16_t segment, uint16_t offset){
    return x86_readbyte_function(segment, offset);
}

uint16_t check_root_file(const char* arr){
    unsigned short i = 0;
    unsigned short j = 0;
    while (j < 224)
    {
        while(i < 11){
            if(arr[i] != read_byte(load_addr_segment, i + 32*j)){
                break;
            }
            i++;
        }
        if(i == 11){
            print("file found!\r\n");
            return (read_byte(load_addr_segment, 32*j + 26) - 2)*1 + 33;    
        }else{ 
            i = 0;
            j++;
        }
    }
    j = 0;
    return 0; 
}

uint16_t check_root_file_cluster(const char* arr){
    unsigned short i = 0;
    unsigned short j = 0;
    while (j < 224)
    {
        while(i < 11){
            if(arr[i] != read_byte(load_addr_segment, i + 32*j)){
                break;
            }
            i++;
        }
        if(i == 11){
            print("file found!\r\n");
            return read_byte(load_addr_segment, 32*j + 26);    
        }else{ 
            i = 0;
            j++;
        }
    }
    j = 0;
    return 0; 
}

void load_file_memory(const char* arr){
    uint16_t lba = 0;
    load_root();
    lba = check_root_file(arr);
    if(lba == 0){
        print("file not found");
    }else{
    load_lba(lba);
    }
}

void switch_protected(){
    print("SWITCHING TO PROTECTED MODE\r\n");
    load_file_memory("KERNLOADBIN");
    x86_protected_mode_jump();
}


/*uint16_t check_root_file(const char* filename) {
    uint16_t j;
    for (j = 0; j < 224; j++) {
        char entry_name[11];
        uint16_t i;
        for (i = 0; i < 11; i++) {
            entry_name[i] = read_byte(load_addr_segment, i + 32 * j);
        }

        // Compare with the target filename
        if (memcmp(entry_name, filename, 11) == 0) {
            print("file found!\r\n");
            return read_byte(load_addr_segment, 11 * j + 26);
        }
    }
    return 50;  // File not found
}*/
