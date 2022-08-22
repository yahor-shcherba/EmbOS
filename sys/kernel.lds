ENTRY(_start)

SECTIONS
{
  . = 1M;

  .hdr :
  {
    *(.hdr)
  }

  .text :
  {
    *(.text)
    *(.rodata)
  }

  .data :
  {
    *(.data)
  }

  .bss :
  {
    *(.bss)
  }

  /DISCARD/ :
  {
    *.(eh-frame)
  }

}
