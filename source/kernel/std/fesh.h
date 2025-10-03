#include "stdint.h"
#define VGA_ADDRESS 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define base_fesh write_VGA("root@fembos>")

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

#define KEYBOARD_DATA_PORT  0x60
#define KEYBOARD_STATUS_PORT 0x64

void clear_screen();
static inline void outb(uint16_t port, uint8_t val);
static inline void outl(uint16_t port, uint32_t val);
static inline uint8_t inb(uint16_t port);
static inline uint32_t inl(uint16_t port);
static inline void wait_for_keypress();
uint8_t get_keypress();
char scan_code_to_ascii(uint8_t scancode);
void set_cursor(uint16_t position);
void put_char_at(char c, uint8_t color, int x, int y);
void write_VGA(char* str);
void handle_inp(char* inp_arr, int start_line);
