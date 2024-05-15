/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 ArturWeber
 *    SPDX-FileCopyrightText: 2024 MVellosa <mvellosa@usp.br>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *    This file is part of SYSeg, available at https://gitlab.com/monaco/syseg.
 */

#include "bios.h"
#include "utils.h"

#define PROMPT "$"		/* Prompt sign.      */
#define PROMPT_ROOT "#"		/* Prompt sign.      */
#define SIZE 15			/* Read buffer size. */

char buffer[SIZE];		/* Read buffer.      */
char buffer_2[SIZE];
char is_root = 0;
#define PASSWORD "admin"

int main()
{
  // clear();

  while (1)
    {
      if (is_root)
        print(PROMPT_ROOT" ");	/* Show prompt.               */
      else
        print(PROMPT" ");		/* Show prompt.               */
      readln(buffer);		/* Read use input.            */

      if (buffer[0])		/* Execute built-in command.  */
        {
          if (!strcmp(buffer,"su")){
            println("password:");
            readln(buffer_2);
            if (!strcmp(buffer_2, PASSWORD)){
              println("Hello root");
              is_root = 1;
            } else {
              println("Denied!");
            }
          } else if (!strcmp(buffer,"end")){
            if (is_root)
              break;
            else
              println("Denied!");
          }
        }
    }
  
  return 0;

}

