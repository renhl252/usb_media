CC=gcc
files=dir_test.c db.c log.c
TARGET=test
all:
	make clean
	$(CC) $(files) -o $(TARGET) -I./build/include -lsqlite3 -L./build/lib -Wl,-rpath=./build/lib

clean:
	rm -f $(TARGET)
	rm -f *.txt
	rm -f /tmp/test.db
	sync
doc:
	man readdir > readdir_help.txt
	man opendir  > opendir_help.txt
	man closedir > closedir_help.txt

.PHONY:all,clean,doc
