
/**************************************************

file: demo_tx.c
purpose: simple demo that transmits characters to
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc demo_tx.c rs232.c -Wall -Wextra -o2 -o test_tx

**************************************************/

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include "rs232.h"



int main()
{
  int
      cport_nr=24,        /* /dev/ttyACM0 (COM1 on windows) */
      bdrate=9600;       /* 9600 baud */

  char c, mode[]={'8','N','1',0};
 

  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");

    return(0);
  }

  while(1)
  {
  	c='F';
  	
    RS232_cputs(cport_nr, &c);

    printf("sent: F\n");

    sleep(5);
    
    c='S';
    
    RS232_cputs(cport_nr, &c);

    printf("sent: S\n");

    sleep(5);

  }

  return(0);
}

