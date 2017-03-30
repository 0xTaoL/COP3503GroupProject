#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
//UPDATED: Now gives an integer for every keyboard input.
//It requires root access though because 
//it needs to read directly from the device "/dev/input/by-path/platform-i8042-serio-0-event-kbd"
//This is the universal linux keyboard device 

//This code is here for possible use in the future, but not necessary
static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

int Keyboard()
{
    const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    struct input_event ev;
    ssize_t n;
    int fd;

    fd = open(dev, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
        return 0;
    }
    while (1) {
        n = read(fd, &ev, sizeof ev);
        if (n == (ssize_t)-1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else
        if (n != sizeof ev) {
            errno = EIO;
            break;
        }
        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2){
	//For some odd reason, the program breaks without "std::cout<<"";", may fix later
	std::cout<<"";
	int KeyCode = (int)ev.code;           
	return KeyCode;
}

    }
    fflush(stdout);
    fprintf(stderr, "%s.\n", strerror(errno));
    return 0;
}

int main(){
	while (true){
		int KeyCode=Keyboard();
		std::cout<<std::endl<<KeyCode;
	}
	return 0;
}
