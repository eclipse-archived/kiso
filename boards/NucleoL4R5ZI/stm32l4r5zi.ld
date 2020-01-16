/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/
/* This file creates the system memory regions and defines their aliases used by 
the linker script */

HIDDEN(KISO_ROM_BASE_1 = 0x08000000);
HIDDEN(KISO_ROM_SIZE_1 = 2M);
HIDDEN(KISO_ROM_TOP_1 = KISO_ROM_BASE_1 + KISO_ROM_SIZE_1);

HIDDEN(KISO_RAM_BASE_1 = 0x20000000);
HIDDEN(KISO_RAM_SIZE_1 = 192K);
HIDDEN(KISO_RAM_TOP_1 = KISO_RAM_BASE_1 + KISO_RAM_SIZE_1);

HIDDEN(KISO_RAM_BASE_2 = 0x20030000);
HIDDEN(KISO_RAM_SIZE_2 = 64K);
HIDDEN(KISO_RAM_TOP_2 = KISO_RAM_BASE_2 + KISO_RAM_SIZE_2);

HIDDEN(KISO_RAM_BASE_3 = 0x20040000);
HIDDEN(KISO_RAM_SIZE_3 = 384K);
HIDDEN(KISO_RAM_TOP_3 = KISO_RAM_BASE_3 + KISO_RAM_SIZE_3);

HIDDEN(KISO_ROM_SIZE = KISO_ROM_SIZE_1);
HIDDEN(KISO_RAM_SIZE = KISO_RAM_SIZE_1 + KISO_RAM_SIZE_2 + KISO_RAM_SIZE_3);
HIDDEN(KISO_ROM_TOP = KISO_ROM_TOP_1);
HIDDEN(KISO_RAM_TOP = MAX(KISO_RAM_TOP_1, MAX(KISO_RAM_TOP_2, KISO_RAM_TOP_3))); 

MEMORY
{
  ROM_PART_1 (rx): o = KISO_ROM_BASE_1, l = KISO_ROM_SIZE_1 /*Flash Partition 1*/
  RAM_ZONE_1 (rwx): o = KISO_RAM_BASE_1, l = KISO_RAM_SIZE_1 /* RAM zone 1*/
  RAM_ZONE_2 (rwx): o = KISO_RAM_BASE_2, l = KISO_RAM_SIZE_2 /* RAM zone 2*/
  RAM_ZONE_3 (rw): o = KISO_RAM_BASE_3, l = KISO_RAM_SIZE_3 /* RAM zone 3*/
}

/* Define Aliases for Memory regions to be used for SECTIONS definition */
REGION_ALIAS("REGION_RO", ROM_PART_1); /* Read only ROM region*/
REGION_ALIAS("REGION_PRO", ROM_PART_1); /* Protected Read only ROM region*/
REGION_ALIAS("REGION_RX", ROM_PART_1); /* Read Execute ROM region*/
REGION_ALIAS("REGION_PRX", ROM_PART_1); /* Protected Read Execute ROM region*/
REGION_ALIAS("REGION_RW", RAM_ZONE_3); /* Read Write RAM region*/
REGION_ALIAS("REGION_PRW", RAM_ZONE_2); /* Protected Read Write RAM region*/
REGION_ALIAS("REGION_RWX", RAM_ZONE_1); /* Protected Read Write RAM region*/
REGION_ALIAS("REGION_PRWX", RAM_ZONE_2); /* Protected Read Write Execute RAM region*/
REGION_ALIAS("REGION_RESET_LOAD_TO", ROM_PART_1); /* region where to execute the reset vector of the device */
REGION_ALIAS("REGION_RESET_LOAD_FROM", ROM_PART_1); /* region where to store the reset vector of the device */

/* symbol definitions */
_estack = KISO_RAM_TOP;

/* Program entry point*/
ENTRY(Reset_Handler)

/* Sections */
SECTIONS
{
  .reset :
  {
    KEEP(*(.isr_vector));
  } > REGION_RESET_LOAD_TO AT>REGION_RESET_LOAD_FROM

  .text :
  {
    PROVIDE(_stext = .);
    *(.text*)
    KEEP(*(.init))
    KEEP(*(.fini))
    *(.ctors*)
    *(.dtors*)
    . = ALIGN(4); /* one could here align to the page size if some firmware update is needed */
    PROVIDE(_etext = .);
  } > REGION_RX


  .rodata :
  {
    . = ALIGN(4);
    *(.rodata*);
    . = ALIGN(4);
  } > REGION_RO

  .data :
  {
    . = ALIGN(4);
    _sdata = . ;
    *(.data*)
    . = ALIGN(4);
    _edata = . ;
  } > REGION_RW AT> REGION_RO
  
    .ARM.exidx : 
  {
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
  } > REGION_RO

  .ARM.extab : 
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > REGION_RO

  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > REGION_RW

  .heap :
  {
    . = ALIGN(4);
    PROVIDE(end = .);
    PROVIDE(_end = .);
    PROVIDE(__end__ = .);
    *(.heap*)

  } > REGION_RW

  .stack : 
  {

  } > REGION_RW

  _sidata = LOADADDR(.data);
  ASSERT(KISO_ROM_SIZE >= ( SIZEOF(.reset) + SIZEOF(.text)+ SIZEOF(.data) + SIZEOF(.ARM.exidx) + SIZEOF(.ARM.extab) ), "ROM OVERFLOW")
  ASSERT(KISO_RAM_SIZE >= ( SIZEOF(.data) + SIZEOF(.bss) + SIZEOF(.heap) ), "RAM OVERFLOW")
}
