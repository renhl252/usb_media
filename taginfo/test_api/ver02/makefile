

INCLUDE := -I./ -I ../../taglib/include/taglib/ 
LIBS:= -L ../../taglib/lib/ -ltag -lz
all:
	g++ media_tag.cpp $(INCLUDE) -c -o media_tag.o
	gcc main.c $(INCLUDE) -c -o main.o
	g++ *.o $(LIBS) -o test_tag
	rm -f *.o
clean:
	rm -rf *.o test_tag

.PHONY:all clean
	