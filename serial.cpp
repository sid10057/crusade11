
#include<unistd.h>
#include<stdio.h>

void publishChar( char c )
{
	FILE *ofs;

	ofs = fopen("/dev/ttyACM0", "w");
	fprintf(ofs,"%c\n", c);
	sleep(1);
	fclose(ofs);
}
int main()
{
	char c;
	

	while(1)
	{
		printf("\nGIVE A CHAR:");
		scanf("%c",&c);
		publishChar(c);
}
	return 0;
}