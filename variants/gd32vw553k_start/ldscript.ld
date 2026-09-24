#include "config_gdm32.h"
#include "mbl_region.h"
#include "wlan_config.h"

OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
    /* Must keep MBL offset - official FLASH layout, required for OTA and secure boot */
    flash (rxai!w) : ORIGIN = RE_FLASH_BASE + RE_IMG_0_OFFSET + IMG_OVERHEAD_LEN, LENGTH = RE_END_OFFSET - (RE_IMG_0_OFFSET + IMG_OVERHEAD_LEN)
    /* SRAM layout: 0x20000200 reserved for ROM, 288K total */
    ram   (wxa!ri) : ORIGIN = RE_SRAM_BASE + RE_IMG_DATA_START, LENGTH = (288K - RE_IMG_DATA_START)
    mib   (ra!wxi) : ORIGIN = 0x40030800, LENGTH = 2K
}

SECTIONS
{
  /* Keep official code_to_sram - critical RF code must run in RAM */
  _sicode = LOADADDR(.code_to_sram);
  .code_to_sram :
  {
    . = ALIGN(4);
    _scode = .;
    *entry.o*(.text.irq)
    *port.o*(.text.eclic_mtip_handler)
    *wrapper_os.o*(.text.sys_memcpy*)
    _ecode = .;
  } >ram AT>flash

  .text :
  {
    *(.rodata .rodata.*)
    *(.text .text.*)
  } >flash AT>flash

  .data :
  {
    PROVIDE( _data = . );
    *(.data .data.*)
    *(.sdata .sdata.*)
  } >ram AT>flash

  .bss :
  {
    PROVIDE( __bss_start = . );
    *(.sbss*) *(.bss .bss.*) *(COMMON)
  } >ram AT>ram

  /* Keep heap/stack layout from official ld */
  .heap ORIGIN(ram) + LENGTH(ram) - __stack_size - __heap_size : { . = __heap_size - 4; } >ram AT>ram
  .stack ORIGIN(ram) + LENGTH(ram) - __stack_size : { . = __stack_size; } >ram AT>ram

  .mib : { *(MACHWMIB) } >mib
}