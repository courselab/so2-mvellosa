/*
 *    SPDX-FileCopyrightText: -y Artur Brenner Weber
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 Artur Brenner Weber
 *    SPDX-FileCopyrightText: 2024 mvellosa <80607728+mvellosa@users.noreply.github.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *    This file is part of SYSeg, available at https://gitlab.com/monaco/syseg.
 */

#include "bios1.h"		/* Function load_kernel . */
#include "kernel.h"		/* Function kmain.        */

int boot()
{
  
  load_kernel();		/* Load the kernel from disk image.  */
  
  kmain();		        /* Call the kernel's entry function. */
  
  return 0;

}

