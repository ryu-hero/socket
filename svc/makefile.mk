### マクロ定義  ###
# compile
CC = gcc

# exe make
TARGET1 = server.exe
PROGS = ${TARGET1}

#src
SRC1 = server.c
ALLSRC = ${SRC1} 

#object
OBJ1 = server.o

#link
LINK = ${OBJ1}

#compile option
CFLAGS = -O2 -Wall

### コンパイル  ###
all:${PROGS}

#SRC1
${OBJ1}:${SRC1}
	${CC} ${SRC1} -c -g -o ${OBJ1} ${CFLAGS}

#link
${TARGET1}:${LINK}
	${CC} ${LINK} -o ${TARGET1}

#delete
clean:
	rm -f ${LINK} ${PROGS}

