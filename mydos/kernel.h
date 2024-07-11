/*
 *    SPDX-FileCopyrightText: 2024 Artur Brenner Weber
 *    SPDX-FileCopyrightText: 2024 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 mvellosa <80607728+mvellosa@users.noreply.github.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *    This file is part of SYSeg, available at https://gitlab.com/monaco/syseg.
 */

#ifndef KERNEL_H
#define KERNEL_H

/* This is kernel's entry function, which is called by the bootloader
   as soon as it loads the kernel from the this image. */

void kmain(void);		

/* This is the command interpreter, which is invoked by the kernel as
   soon as the boot is complete. 
   
   Our tiny command-line parser is too simple: commands are ASCII single words
   with no command line arguments (no blanks). */


void shell();			/* Command interpreter. */
#define BUFF_SIZE 64		/* Max command length.  */
#define PROMPT "> "		/* Command-line prompt. */

#define DIR_ENTRY_LEN 32 	  /* Max file name length in bytes.           */
#define FS_SIGNATURE "\xeb\xety"  /* File system signature.                   */
#define FS_SIGLEN 4               /* Signature length.                        */

#define NUM_CYLINDERS 80
#define NUM_SECTORS 63
#define NUM_HEADS 2
#define SECTOR_SIZE 512

#define NULL ((void *)0)


/* Built-in commands. */

void f_help();
void f_exec();
void f_quit();
void f_list();

extern struct cmd_t
{
  char name[32];
  void (*funct)();
} cmds[];

#endif  /* KERNEL_H  */
