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

/* This source file implements the kernel entry function 'kmain' called
   by the bootloader, and the command-line interpreter. Other kernel functions
   were implemented separately in another source file for legibility. */

#include "bios1.h"		/* For kwrite() etc.            */
#include "bios2.h"		/* For kread() etc.             */
#include "kernel.h"		/* Essential kernel functions.  */
#include "kaux.h"		/* Auxiliary kernel functions.  */

/* Kernel's entry function. */


/* The file header. */
struct fs_header_t {
  unsigned char  signature[FS_SIGLEN];    /* The file system signature.              */
  unsigned short total_number_of_sectors; /* Number of 512-byte disk blocks.         */
  unsigned short number_of_boot_sectors;  /* Sectors reserved for boot code.         */
  unsigned short number_of_file_entries;  /* Maximum number of files in the disk.    */
  unsigned short max_file_size;		  /* Maximum size of a file in blocks.       */
  unsigned int unused_space;              /* Remaining space less than max_file_size.*/
} __attribute__((packed)) fs_header;      /* Disable alignment to preserve offsets.  */

// struct _fs_header_t *header_fs = NULL;

void kmain(void)
{
  int i, j;  
  register_syscall_handler();	/* Register syscall handler at int 0x21.*/

  splash();			/* Uncessary spash screen.              */

  shell();			/* Invoke the command-line interpreter. */
  
  halt();			/* On exit, halt.                       */
  
}


// void load_fs_header(){
//   header_fs = (struct _fs_header_t *) 0x7c00;
// }


void load_disk(int sector, int num_entries, void *addres) {
  	__asm__ volatile(
		"pusha \n"
      	"mov boot_drive, %%dl \n"    	/* Select the boot drive (from rt0.o) */
      	"mov $0x2, %%ah \n"          	/* BIOS disk service: op. read sector */
      	"mov %[NumEntr], %%al \n" 		/* How many sectors to read          */
      	"mov $0x0, %%ch \n"          	/* Cylinder (starts at 0)  */
      	"mov %[Sect], %%cl \n"  		/* Sector   (starts at 1)  */
      	"mov $0x0, %%dh \n"          	/* Head     (starts at 0)      */
      	"mov %[Addr], %%bx \n" 			/* Where to load the file system (rt0.o)   */
      	"int $0x13 \n"               	/* Call BIOS disk service 0x13        */
      	"popa \n" ::
    	[Sect] "g"(sector),
      	[NumEntr] "g"(num_entries),
      	[Addr] "g"(addres));
}

void load_disk_2_ram(int LBA, int num_sectors_read, void *out_addres) {
  // convert LBA to CHS
  int C = LBA / (NUM_HEADS * NUM_SECTORS);
  int S = (LBA % NUM_SECTORS) + 1;
  int H = (LBA / NUM_SECTORS) % NUM_HEADS;

  // from load_keren in bios1.S, just making it more generic 
  // (idea gotten from telegram thread)
  __asm__ volatile(
  "pusha \n"
  "mov boot_drive, %%dl \n"    /* Select the boot drive (from rt0.o). */
  "mov $0x2, %%ah \n"          /* BIOS disk service: op. read sector. */
  "mov %[sectors2Read], %%al \n" /* How many sectors to read          */
  "mov %[cylinderCoordinate], %%ch \n" /* Cylinder coordinate (starts at 0).  */
  "mov %[sectorCoordinate], %%cl \n"  /* Sector coordinate   (starts at 1).  */
  "mov %[headCoordinate], %%dh \n"  /* Head coordinate     (starts at 0).      */
  "mov %[outAddr], %%bx \n" /* Where to load the file system (rt0.o).   */
  "int $0x13 \n"               /* Call BIOS disk service 0x13.        */
  "popa \n" ::
  [cylinderCoordinate] "g"(C),
  [sectorCoordinate] "g"(S),
  [headCoordinate] "g"(H),
  [sectors2Read] "g"(num_sectors_read),
  [outAddr] "g"(out_addres));
}

/* Tiny Shell (command-line interpreter). */

char buffer[BUFF_SIZE];
int go_on = 1;

void shell()
{
  int i;
  clear();
  kwrite ("TinyDOS 1.0\n");

  while (go_on)
    {

      /* Read the user input. 
	 Commands are single-word ASCII tokens with no blanks. */
      do
	{
	  kwrite(PROMPT);
	  kread (buffer);
	}
      while (!buffer[0]);

      /* Check for matching built-in commands */
      
      i=0;
      while (cmds[i].funct)
	{
	  if (!strcmp(buffer, cmds[i].name))
	    {
	      cmds[i].funct();
	      break;
	    }
	  i++;
	}

      /* If the user input does not match any built-in command name, just
	 ignore and read the next command. If we were to execute external
	 programs, on the other hand, this is where we would search for a 
	 corresponding file with a matching name in the storage device, 
	 load it and transfer it the execution. Left as exercise. */
      
      if (!cmds[i].funct)
	kwrite ("Command not found\n");
    }
}


/* Array with built-in command names and respective function pointers. 
   Function prototypes are in kernel.h. */

struct cmd_t cmds[] =
  {
    {"help",    f_help},     /* Print a help message.       */
    {"quit",    f_quit},     /* Exit TyDOS.                 */
    {"exec",    f_exec},     /* Execute an example program. */
    {"list",    f_list},     /* Execute an example program. */
    {0, 0}
  };


/* Build-in shell command: help. */

void f_help()
{
  kwrite ("...me, Obi-Wan, you're my only hope!\n\n");
  kwrite ("   But we can try also some commands:\n");
  kwrite ("      list    (to list files in file system directory\n");
  kwrite ("      exec    (to execute an user program example\n");
  kwrite ("      quit    (to exit TyDOS)\n");
}

void f_quit()
{
  kwrite ("Program halted. Bye.");
  go_on = 0;
}

void f_exec()
{
  return;
} 

void f_list()
{
  // load_fs_header();
  struct fs_header_t *header_fs = (struct fs_header_t *)0x7c00;

  int sector_start_coord = header_fs->number_of_boot_sectors; // start reading after boot sectors
  int entries_size_sectors = header_fs->number_of_file_entries * DIR_ENTRY_LEN / SECTOR_SIZE;

  extern char _MEM_POOL;
  void *directory_addres = (void *)&_MEM_POOL;

  load_disk_2_ram(sector_start_coord, entries_size_sectors, directory_addres);

  for (int i = 0; i < 1; i++) {
    char *file_name = directory_addres + i * DIR_ENTRY_LEN; // point to beggining of file name
    kwrite(file_name);
    kwrite(" ");
  }

  return;
}