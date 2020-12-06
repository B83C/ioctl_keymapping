#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>

#include "config.h"

int map(int fd)
{
    int ret;
    for(int i = 0; i < sizeof(scancode_keycodes)/sizeof(scancode_keycodes[0]); i++)
    {
	int test[2];
	test[0] = scancode_keycodes[i][0];
	ret = ioctl(fd, EVIOCGKEYCODE, test);
	printf("%d\n", test[1] );
	ret = ioctl(fd, EVIOCSKEYCODE, scancode_keycodes[i]);	
	if(ret < 0)
	{
	    fprintf(stderr, "couldn't map keycode: %d, error: %s\n", scancode_keycodes[i][0], strerror(-ret));
	    return -1;
	}
    }
    return 0;
}

int unmap(int fd)
{
    int ret;
    for(int i = 0; i < sizeof(scancode_keycodes)/sizeof(scancode_keycodes[0]); i++)
    {
	scancode_keycodes[i][1] = scancode_keycodes[i][0];
	ret = ioctl(fd, EVIOCSKEYCODE, scancode_keycodes[i]);
	if(ret < 0)
	{
	    fprintf(stderr, "couldn't unmap keycode: %d, error: %s\n", scancode_keycodes[i][0], strerror(-ret));
	    return -1;
	}
    }
    return 0;
}

int checkname(char* name)
{
    for(int i = 0; i < sizeof(keyboards)/sizeof(keyboards[0]); i++)
    {
	if(!strcmp(name, keyboards[i]))
	    return 0;	    
    }
    return -1;
}

int main(int argc, char **argv)
{
    for(int i = 2; i < argc; i++)
    {
	int fd;
	int ret;
	char name[256];
	unsigned int bit;
	fd = open(argv[i], O_CLOEXEC);
	ret = ioctl(fd, EVIOCGBIT(0,sizeof(bit)),&bit);
	if(ret < 0)
	{
	    fprintf(stderr, "Error getting EV bits for %s, error: %s\n", argv[i], strerror(-ret));
	    goto SKIPPED;
	}
	ret = ioctl (fd, EVIOCGNAME(sizeof(name)), name);
	if(ret < 0)
	{
	    fprintf(stderr, "Error getting device name for %s, error: %s\n", argv[i], strerror(-ret));
	    goto SKIPPED;
	}
	if((bit & 0x120013) == 0x120013 && !checkname(name))
	{
	   if(*argv[1] - '0') ret = map(fd); 
	   else ret = unmap(fd);
	   if(!ret)
	   {
	       printf("Keys mapped/unmapped successfully for %s\n", argv[i]);
	   }
	}
SKIPPED:
	close(fd);
    }
}
