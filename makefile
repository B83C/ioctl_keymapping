SRC = key.c
OBJ = ${SRC:.c=.o}
CC = cc
PREFIX = /usr/local

all: key config.h

config.h:
	cp config.def.h $@

${OBJ} = config.h

clean:
	rm -f key ${OBJ}

$(OBJ): %.o : %.c 
	${CC} -c $< ${CFLAGS}

key: $(OBJ)
	${CC} -o $@ ${OBJ} ${LDFLAGS}
	
install: key
	sudo cp ./key ${PREFIX}/bin/
	sudo cp ioctlkeymapping.service /etc/systemd/system/
	sudo chmod 644 /etc/systemd/system/ioctlkeymapping.service
	sudo systemctl enable ioctlkeymapping

uninstall:
	sudo rm ${PREFIX}/bin/key
	sudo systemctl disable ioctlkeymapping
	sudo rm /etc/systemd/system/ioctlkeymapping.service

.PHONY: all
