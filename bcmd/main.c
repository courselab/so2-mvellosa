/*
 *    SPDX-FileCopyrightText: 2021 Monaco F. J. <monaco@usp.br>
 *    SPDX-FileCopyrightText: 2024 MVellosa <mvellosa@usp.br>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative work from SYSeg (https://gitlab.com/monaco/syseg)
 *  and contains modifications carried out by the following author(s):
 *  MVellosa <mvellosa@usp.br>
 */

#include "bios.h"
#include "utils.h"

#define PROMPT "$ "		/* Prompt sign.      */
#define SIZE 20			/* Read buffer size. */

char buffer[SIZE];		/* Read buffer.      */

int main()
{
  clear();
  
  println  ("Boot Command 1.0");

  unsigned char current_time[3]; // Array para armazenar horas, minutos e segundos

  // Chamando a função assembly
  get_current_time(current_time);

  // Exibindo a hora atual
  println("Hora atual: %02d:%02d:%02d\n", current_time[2], current_time[1], current_time[0]);


  while (1)
    {
      print(PROMPT);		/* Show prompt.               */
      readln(buffer);		/* Read use input.            */

      if (buffer[0])		/* Execute built-in command.  */
	{
	  if (!strcmp(buffer,"help"))
	    println("A Beattles's song.");
	  else 
	    println("Unkown command.");
	}
    }
  
  return 0;

}

