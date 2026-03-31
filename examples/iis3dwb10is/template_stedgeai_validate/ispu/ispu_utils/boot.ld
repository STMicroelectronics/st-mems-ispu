/* OUTPUT_ARCH(reisc20) */
ENTRY(_startup) /* crt0 entry point */
MEMORY
{
    DATA_RAM (arwx) : ORIGIN = 0x02000, LENGTH = 56K /* data ram, 32 KB + 8 KB + 8 KB + 8 KB */
    CODE_RAM (arwx) : ORIGIN = 0x40000, LENGTH = 32K /* code ram, 32 KB */
    ISPU     (rw)   : ORIGIN = 0x80000, LENGTH = 256 /* registers for ISPU HW functions */
    CTRL     (rw)   : ORIGIN = 0x80800, LENGTH = 256 /* control registers */
    EREG     (rw)   : ORIGIN = 0x80900, LENGTH = 256 /* external registers */
}

GROUP(-lsim -lc -lm -lgcc)

SECTIONS
{
    .init :
    {
        *(.init)
        . = ALIGN(4);
    } > CODE_RAM

    .text :
    {
        *(.text .text.* .gnu.linkonce.t.*)
        . = ALIGN(4);
        PROVIDE(_etext = .);
    } > CODE_RAM

    .rodata :
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        . = ALIGN(4);
        PROVIDE(_erodata = .);
    } > CODE_RAM

    PROVIDE(_bss_start = .);
    PROVIDE(__bss_start = .);
    .bss :
    {
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(4);
        PROVIDE(_ebss = .);
    } > DATA_RAM
    PROVIDE(_end = .);

    .ram : {
        *(.ram)
        . = ALIGN(4);
    } > DATA_RAM

    .data :
    {
        *(.data .data.* .gnu.linkonce.d.*)
        . = ALIGN(4);
        _data = .;
        PROVIDE(_edata = .);
    } > DATA_RAM

    /* Stabs debugging sections.  */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }
    /* DWARF debug sections.
       Symbols in the DWARF debugging sections are relative to the beginning
       of the section so we begin them at 0.  */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }
    /* DWARF 3 */
    .debug_pubtypes 0 : { *(.debug_pubtypes) }
    .debug_ranges   0 : { *(.debug_ranges) }

/***************************************************************************************/
/* global constructors destructors                                                     */
/***************************************************************************************/

  .ctors          :
  {
    __ctors = .;
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __ctors_end = .;
  } > DATA_RAM
  .dtors          :
  {
    __dtors = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __dtors_end = .;
  } > DATA_RAM

  PROVIDE(_canary_addr = .);
  .canary :
  {
    BYTE(0xA5)
  } > DATA_RAM
}

__ispu_reg_base = ORIGIN(ISPU);
__ctrl_reg_base = ORIGIN(CTRL);
__ctrl_reg_ext  = ORIGIN(EREG);
_data_ram_begin = ORIGIN(DATA_RAM);
_data_ram_end   = ORIGIN(DATA_RAM) + LENGTH(DATA_RAM);
_code_ram_begin = ORIGIN(CODE_RAM);
_code_ram_end   = ORIGIN(CODE_RAM) + LENGTH(CODE_RAM);
_stack          = _data_ram_end;

BSS_VMA         = ADDR(.bss);
TEXT_VMA        = ADDR(.text);          TEXT_LMA        = LOADADDR(.text);
RODATA_VMA      = ADDR(.rodata);        RODATA_LMA      = LOADADDR(.rodata);
DATA_VMA        = ADDR(.data);          DATA_LMA        = LOADADDR(.data);

_arg0 = DATA_LMA;
_arg1 = DATA_LMA + 0x000004;
_arg2 = DATA_LMA + 0x000008;
_arg3 = DATA_LMA + 0x00000c;

ADDR_FOR_SLEEP  = 0xC0000;

