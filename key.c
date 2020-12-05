#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>

unsigned scancode_keycodes[][2] = {
    {KEY_KP1, KEY_LEFT},
    {KEY_KP2, KEY_DOWN},
    {KEY_KP3, KEY_UP},
    {KEY_KPMINUS, KEY_RIGHT}
};

int main(int argc, char **argv)
{
    int fd;
    int ret;
    unsigned scancode_keycode[2];

    fd = open("/dev/input/event4", O_CLOEXEC);
    if (fd < 0) {
	fprintf(stderr, "couldn't open device: %s\n", strerror(-fd));
	goto err;
    }

    for(int i = 0; i < sizeof(scancode_keycodes)/sizeof(scancode_keycodes[0]); i++)
    {
	ret = ioctl(fd, EVIOCSKEYCODE, scancode_keycodes[i]);
	if (ret < 0) {
	    fprintf(stderr, "couldn't set keycode: %s\n", strerror(-ret));
	    goto err_fd;
	}
    }

    return 0;

err_fd:
    close(fd);
err:
    return 1;
}
