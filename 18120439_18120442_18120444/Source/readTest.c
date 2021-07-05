#include <stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#define buffLength 6
static char readResult[buffLength];

int main()
{
    int r,fd;
    fd= open("/dev/myrand",O_RDWR);
    if(fd<0)
    {
        perror("Open failed\n");
        return -1;
    }
    	printf("Device opened\n");
	printf("Device is running\n");
    r=read(fd,readResult,buffLength);
    if(r<0)
    {
        perror("Read failed\n");
    }
    printf("Your random number: %s\n",readResult);
    printf("Device closed\n");
    close(fd);
    return 0;
}
