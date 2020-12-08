SRC = key.c
OBJ = ${SRC:.c=.o}
CC = cc
PREFIX = /usr/local/

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
	
.PHONY: all
