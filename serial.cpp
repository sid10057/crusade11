
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void publishChar( char c )
{
	FILE *ofs;

	ofs = fopen("/dev/ttyACM0", "r+");
	fprintf(ofs,"%c\n", c);
	sleep(1);
	fclose(ofs);
}
int main()
{
	char c;
	
		printf("\nGIVE A CHAR:");
		scanf("%c",&c);
		publishChar(c);
	return 0;
}