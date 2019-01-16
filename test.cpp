#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include "rs232.h"
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<bits/stdc++.h>
using namespace cv;
using namespace std;
int main()
{
	
    int cport_nr=24;   /* /dev/ttyACM0 (COM1 on windows) */
    int bdrate=9600;       /* 9600 baud */
	char c, mode[]={'8','N','1',0};
	 if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");

    return(0);
  }
  	
  	c='K';
	RS232_cputs(cport_nr,&c);
	waitKey(0);
	printf("fffff\n");
	//c='S';
	//RS232_cputs(cport_nr,&c);
	//waitKey(100);
	return 0;
}	