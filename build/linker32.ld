ENTRY(entry)
OUTPUT_FORMAT("binary")
phys = 0x00007C00;

SECTIONS
{
    . = phys;

    .entry              : { __entry_start = .;      *(.entry)   }
    .text               : { __text_start = .;       *(.text)    }
    .data               : { __data_start = .;       *(.data)    }
    .rodata             : { __rodata_start = .;     *(.rodata)  }
    .bss                : { __bss_start = .;        *(.bss)     }  
    .custom_data        : { __custom_data = .;      *(.custom_data)}
    
    __end = .;
}
