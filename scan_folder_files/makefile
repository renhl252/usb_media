CC=gcc
files=dir_test.c
TARGET=test
all:
	make clean
	$(CC) $(files) -o $(TARGET)

clean:
	rm -f $(TARGET)
	rm -f *.txt

doc:
	man readdir > readdir_help.txt
	man opendir  > opendir_help.txt
	man closedir > closedir_help.txt

.PHONY:all,clean,doc