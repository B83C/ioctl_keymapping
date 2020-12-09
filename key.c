#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>
#include <dirent.h>

#include "config.h"

int map(int fd)
{
    int ret;
    for(int i = 0; i < sizeof(scancode_keycodes)/sizeof(scancode_keycodes[0]); i++)
    {
	int test[2];
	test[0] = scancode_keycodes[i][0];
	ret = ioctl(fd, EVIOCGKEYCODE, test);
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
    int fdr = open("/dev/input/", O_DIRECTORY);
    DIR* ed = fdopendir(fdr);
    struct dirent* file;
    while((file = readdir(ed)) != NULL)
    {
    if(file->d_type != DT_CHR)	continue;
	int fd;
	int ret;
	char name[256];
	unsigned int bit;
	fd = openat(fdr, file->d_name, O_CLOEXEC);
	//Added in order to decrease the amount of syscalls
	#ifdef DOUBLE_CHECK
	ret = ioctl(fd, EVIOCGBIT(0,sizeof(bit)),&bit);
	if(ret < 0)
	{
	    fprintf(stderr, "Error getting EV bits for %s, error: %s\n", file->d_name, strerror(-ret));
	    goto SKIPPED;
	}
	#endif
	ret = ioctl (fd, EVIOCGNAME(sizeof(name)), name);
	if(ret < 0)
	{
	    fprintf(stderr, "Error getting device name for %s, error: %s\n", file->d_name, strerror(-ret));
	    goto SKIPPED;
	}
	if(
		#ifdef DOUBLE_CHECK
		(bit & 0x120013) == 0x120013 && 
		#endif
		!checkname(name))
	{
	    if(*argv[1] - '0') ret = map(fd); 
	    else ret = unmap(fd);
	    if(!ret)
	    {
		printf("Keys mapped/unmapped successfully for %s\n", file->d_name);
	    }
	}
SKIPPED:
	close(fd);
    }
    return 0;
}
