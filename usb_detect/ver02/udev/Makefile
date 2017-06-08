# specify the compiler
CC=gcc

# specify library

INCFLAGS=-I ./ 

TARGET=test
# specify library
LIBFLAGS=-lpthread -ludev

# specify additional compile flags
#FLAGS= -lm -g -Wall -Wextra 

# List of files specific 
SRC:= Udev_Monitor.c              
 
testapp:
	 ${CC} -o ${TARGET} *.c ${LIBFLAGS} ${INCFLAGS}

clean:
	 rm -f ${TARGET}
