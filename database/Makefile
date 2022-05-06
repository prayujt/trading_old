CC = g++
CFLAGS = -std=c++11 -w -g -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi
UNAME_S := $(shell uname -s)

TARGET = main
SOURCES := main.cpp

ifeq ($(UNAME_S),Darwin)
	CFLAGS += -lmongocxx -lbsoncxx
endif

MAKE_CMD = $(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

ifeq ($(UNAME_S),Linux)
	MAKE_CMD += /usr/local/lib/libmongocxx.so.3.6.7 /usr/local/lib/libbsoncxx.so.3.6.7
endif

all:
	$(MAKE_CMD)
clean:
	rm $(TARGET)
	rm -r $(TARGET).dSYM
