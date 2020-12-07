SRC = key.c
OBJ = ${SRC:.c=.o}
CC = cc
PREFIX = /usr/local/

config.h:
	cp config.def.h $@

${OBJ} = config.h

clean:
	rm -f key ${OBJ}

$(OBJ): %.o : %.c 
	${CC} -c $< ${CFLAGS}

key:
	${CC} -o $@ ${OBJ} ${LDFLAGS}

install:
	
