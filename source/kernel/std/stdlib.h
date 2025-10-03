#include "stdint.h"
void putc(char c); //puts charachter c on screen
void print(const char* str); //puts str on screen
void load_lba(uint16_t lba); //loads specified segment by lba into memory at 0x0050:0x0000
void load_root(); //loads root directory into memory at 0x0050:0x0000
uint16_t read_byte(uint16_t segment, uint16_t offset); //reads byte at segment:offset
uint16_t check_root_file(const char* arr); //checks for specified file in root directory and returns lba number of the file if found, returns 0 otherwise
uint16_t check_root_file_cluster(const char* arr); //checks for specified file in root directory and returns cluster number of the file if found, returns 0 otherwise
void load_file_memory(const char* arr); //loads the specified file name into the memory address 0x0050:0x0000
void switch_protected(); //switches to protected mode at address 0x00007c00 in 32 bit mode and runs KERNEL32.BIN
//function to read and ouput text from a text file aka(text file reader)
//implement support for other files
//write a scripting language
//currrently create support for 1 file only. at a time.