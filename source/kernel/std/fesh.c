#include "stdint.h"
#include "fesh.h"

int start_line = 19;
void clear_screen() {
    volatile char *video_memory = (volatile char *)VGA_ADDRESS;

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i * 2] = ' ';   // Character: Space (' ')
        video_memory[i * 2 + 1] = 0x07; // Attribute: White text on black background
    }

}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outl(uint16_t port, uint32_t value) {
    asm volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint32_t inl(uint16_t port) {
    uint32_t value;
    asm volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void wait_for_keypress() {
    while ((inb(KEYBOARD_STATUS_PORT) & 1) == 0);
}

uint8_t get_keypress() {
    wait_for_keypress(); // Wait until a key is pressed
    return inb(KEYBOARD_DATA_PORT); // Read scan code
}

char scan_code_to_ascii(uint8_t scancode) {
    static char key_map[128] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
        '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
        '\n', 0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 
        0,  ' ', 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };

    if (scancode < 128) {
        return key_map[scancode];
    }
    return 0;
}


void set_cursor(uint16_t position) {
    outb(0x3D4, 0x0E);           // Cursor high byte
    outb(0x3D5, (position >> 8) & 0xFF);
    outb(0x3D4, 0x0F);           // Cursor low byte
    outb(0x3D5, position & 0xFF);
}

void put_char_at(char c, uint8_t color, int x, int y) {
    volatile char *video_memory = (volatile char *)VGA_ADDRESS;
    int position = (y * SCREEN_WIDTH + x) * 2; // VGA memory is word (2-byte) addressed

    video_memory[position] = c;       // Character byte
    video_memory[position + 1] = color; // Attribute byte (text color)
}

void print_hex(uint16_t value, int x, int y) {
    char hex_digits[] = "0123456789ABCDEF";
    
    put_char_at('0', 0x0F, x, y);
    put_char_at('x', 0x0F, x+1,y);

    for (int i = 12; i >= 0; i -= 4) {
        put_char_at(hex_digits[(value >> i) & 0xF], 0x0F, x+1+i, y);
    }
}


void write_VGA(char* str){
    int i = 0;
    if(start_line == 25){
        clear_screen();
        while(*str){
            put_char_at(*str, 0x0F, i, 0);
            i += 1;
            str++;
        }
        start_line = 1;
    }else{
    while(*str)
    {
        put_char_at(*str, 0x0F, i, start_line);
        i += 1;
        str++;
    }
    start_line += 1;
}
}
uint32_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t address;
    uint32_t result;

    // Ensure offset is aligned to 4 bytes (since PCI registers are 32-bit)
    offset &= 0xFC;

    // Construct the PCI configuration address
    address = (1U << 31)      // Enable bit (bit 31 must be set)
            | (bus << 16)     // Bus number (8 bits)
            | (device << 11)  // Device number (5 bits)
            | (function << 8) // Function number (3 bits)
            | (offset & 0xFC); // Register offset (aligned)

    // Write address to CONFIG_ADDRESS (0xCF8)
    outl(PCI_CONFIG_ADDRESS, address);

    // Read result from CONFIG_DATA (0xCFC)
    result = inl(PCI_CONFIG_DATA);

    return result;
}
uint32_t get_e1000_mmio_base(uint8_t bus, uint8_t slot) {
    uint32_t bar0 = pci_read(bus, slot, 0, 0x10); // Read BAR0

    if (bar0 & 1) {
        write_VGA("I/O port");// It's an I/O port address, not MMIO
        return 0;
    }
    else{
        write_VGA("MMIO DEVICE");
    }

    return bar0 & 0xFFFFFFF0; // Mask out lower 4 bits
}


void scan_pci_devices() {
    write_VGA("Scanning PCI devices..");

    for (uint8_t bus = 0; bus < 256; bus++) {          // Loop through buses
        for (uint8_t device = 0; device < 32; device++) { // Loop through devices
            for (uint8_t function = 0; function < 8; function++) { // Loop through functions

                // Read Vendor ID (offset 0x00)
                uint32_t data = pci_read(bus, device, function, 0x00);
                uint16_t vendor_id = data & 0xFFFF;
                uint16_t device_id = (data >> 16) & 0xFFFF;

                if (vendor_id == 0x8086 && device_id == 0x100E) { // 0xFFFF means no device present
                    write_VGA("Found PCI Device");
                    get_e1000_mmio_base(bus, device);

                }
            }
        }
    }
}






void main_fesh(){
    scan_pci_devices();
    //while(1){
    //     char inp[70];
    //     for (int i = 0; i < 70; i++)
    //     {
    //         inp[i] = 0;
    //     }
        
    //     char c = 0;
    //     int j = 1;
    //     set_cursor(SCREEN_WIDTH*(start_line-1) + 13);
    //     while (1) {
    //         uint8_t scancode = get_keypress(); // Read scan code
    //         c = scan_code_to_ascii(scancode);
    //         if (c) {
    //             if(inb(KEYBOARD_DATA_PORT) == 0x1C){
    //                 handle_inp(inp, start_line);
    //                 break;
    //             }else if (inb(KEYBOARD_DATA_PORT) == 0x0E)
    //             {
    //                 if(j == 1 || j == 0){
    //                     j = 1;
    //                 }else{
    //                     inp[j-1] = 0;
    //                     j -= 1;
    //                 }
    //                 set_cursor(SCREEN_WIDTH*(start_line-1) + 12 + j);
    //                 put_char_at(' ',0x0F, 12+j,start_line-1);
    //             }
    //             else{
    //             put_char_at(c, 0x0F, 12+j, start_line-1); // You need to implement this for output
    //             inp[j-1] = c;
    //             set_cursor(SCREEN_WIDTH*(start_line-1) + 13 + j);
    //             j += 1;
    //             }
    //         }
    //     }
    // }
}