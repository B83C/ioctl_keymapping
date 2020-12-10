# ioctl_keymapping
Simple keymapping employing ioctl which works in both X11 and console. Not a daemon, its execution is one-shot.

## INSTALLATION
Paste the commands into your terminal and hit enter
```
git clone https://github.com/b83c/ioctl_keymapping
cd ioctl_keymapping
make clean && make install #This will install the binary file into /usr/local/bin and at the same time, create a systemd service.
```

## CONFIGURATION
By default, config.h doesn't exist in the repository, and is copied from config.def.h at compile time. And of course, you can manually make a copy of your own.
Below is an example that works on most laptops:
```c
//Format: {KEY_TO_BE_MAPPED, MAPPED_KEY}
static unsigned scancode_keycodes[][2] = {
    {KEY_KP1, KEY_LEFT},
    {KEY_KP2, KEY_DOWN},
    {KEY_KP3, KEY_UP},
    {KEY_KPMINUS, KEY_RIGHT}
};

static char* keyboards[] = {
    "AT Translated Set 2 keyboard" //Do xinput list to retrieve your keyboard name.
}
```

## TODO
Reduce syscalls and boost performance.

## LICENSE
none
