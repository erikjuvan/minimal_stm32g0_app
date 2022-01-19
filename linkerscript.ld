/* ------------------------------------------------------------ */
/*                        Linkerscript                          */
/*                      for NUCLEO-G0B1RE                       */
/* ------------------------------------------------------------ */

RAM_SIZE = 128k; /* 144k - 16k (SRAM parity enabled) */

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM (xrw) : ORIGIN = 0x20000000, LENGTH = RAM_SIZE
}

/* The ENTRY(..) directive overrides the default entry point symbol _start.
 * In fact, the ENTRY(..) directive is meaningless for embedded chips,
 * but it is informative for debuggers. */
ENTRY(_start)

SECTIONS
{
    /* Program code into FLASH */
    .text : ALIGN(4)
    {
        *(.isr_vector)          /* Vector table */
        *(.text)                /* Program code */
        *(.text*)               /* Merge all .text.* sections inside the .text section */
        KEEP(*(.isr_vector))    /* Don't allow other tools to strip this off */
    } >FLASH


    _sidata = LOADADDR(.data);  /* Used by startup code to initialize data */

    .data : ALIGN(4)
    {
        . = ALIGN(4);
        _sdata = .;             /* Create a global symbol at data start */

        *(.data)
        *(.data*)

        . = ALIGN(4);
        _edata = .;             /* Define a global symbol at data end */
    } >SRAM AT >FLASH

}
