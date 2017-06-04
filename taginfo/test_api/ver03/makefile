CC = gcc
CXX = g++

ECHO = echo -e
MKDIR = mkdir -p

OUTDIR = $(PWD)/obj

INC := 	../../taglib/include/taglib/

SRC :=  ./*.c  \
        ./*.cpp

INCLUDE:=$(INC:%=-I%)

SRC_FILES=$(wildcard $(SRC))
OBJS:=$(patsubst %.c,%.o, $(filter %.c,$(SRC_FILES)))
OBJS += $(patsubst %.cpp,%.o, $(filter %.cpp,$(SRC_FILES)))
BUILD_OBJS = $(OBJS:%.o=$(OUTDIR)/%.o)

LIBS:= -lpthread -lrt -L../../taglib/lib/ -ltag -lz -lsqlite3

TARGET= mplayer

all:
	make clean
	make $(TARGET)


$(TARGET):$(BUILD_OBJS)
	$(CXX) $(BUILD_OBJS) $(INCLUDE) -o $@ $(LIBS) -g
	cp test.db_bk test.db

$(OUTDIR)/%.o:%.c
	@(test -e $(@D) )|| ( $(MKDIR) $(@D) && $(ECHO) ...mkdir $(@D))
	$(CC) -c $^ $(INCLUDE) -o $@

$(OUTDIR)/%.o:%.cpp
	@(test -e $(@D) ) || ( $(MKDIR) $(@D) && $(ECHO) ...mkdir $(@D))
	$(CXX) -c $^ $(INCLUDE) -o $@

clean:
	rm -f $(TARGET)
	rm -f $(BUILD_OBJS)
	rm -rf $(OUTDIR)
	rm -rf test.db

$(TEST_TARGET):
	$(CC) $(SRC_TEST_MP) $(INCLUDE) -o $@ $(LIBS) -g

.PHONY : all clean $(TEST_TARGET) $(TARGET) 