CC = gcc

TARGET=recv

FILES=recv.c ../common/log.c
INC=-I../common/
LIBS=-lrt

all:
	make clean
	$(CC)  $(FILES) $(INC) $(LIBS) -o $(TARGET) 


clean:
	rm -rf $(TARGET) 

exe:
	./$(TARGET)&

.PHONY:all, clean, exe
	
