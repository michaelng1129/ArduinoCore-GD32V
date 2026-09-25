OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
    /* 2M version - LENGTH limited to 2M for 2M flash ICs */
    flash (rxai!w) : ORIGIN = 0x0800A000, LENGTH = 0x1F6000
    ram   (wxa!ri) : ORIGIN = 0x20000200, LENGTH = 0x47E00
    mib   (ra!wxi) : ORIGIN = 0x40030800, LENGTH = 2K
}

SECTIONS
{
  __stack_size = DEFINED(__stack_size) ? __stack_size : 4K;
  __heap_size = DEFINED(__heap_size) ? __heap_size : 16K;
  __trace_size = DEFINED(__trace_size) ? __trace_size : 4;
  .init : { _vector_base = .; KEEP (*(SORT_NONE(.init))) } >flash AT>flash
  _sicode = LOADADDR(.code_to_sram);
  .code_to_sram : { . = ALIGN(4); _scode = .; *entry.o*(.text.irq) *port.o*(.text.eclic_mtip_handler) _ecode = .; } >ram AT>flash
  .text : { *(.rodata .rodata.*) *(.text .text.*) } >flash AT>flash
  .data : { PROVIDE( _data = . ); *(.data .data.*) } >ram AT>flash
  .bss : { PROVIDE( __bss_start = . ); *(.bss .bss.*) *(COMMON) } >ram AT>ram
  .heap ORIGIN(ram) + LENGTH(ram) - __stack_size - __heap_size : { . = __heap_size - 4; } >ram AT>ram
  .stack ORIGIN(ram) + LENGTH(ram) - __stack_size : { . = __stack_size; } >ram AT>ram
  .mib : { *(MACHWMIB) } >mib
}
