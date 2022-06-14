CC = g++
CFLAGS = -std=c++17 -w -g -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -lmongocxx -lbsoncxx -lalpaca
LIBS = -lssl -lcrypto -lglog
# UNAME_S := $(shell uname -s)

TARGET = main
SOURCES := database/*.cpp algo/*.cpp

# ifeq ($(UNAME_S),Darwin)
	# CFLAGS += -lmongocxx -lbsoncxx -lssl -lcrypto -lglog
# endif

MAKE_CMD = $(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# ifeq ($(UNAME_S),Linux)
	# MAKE_CMD += /usr/local/lib/libmongocxx.so /usr/local/lib/libbsoncxx.so
# endif
project:
	$(MAKE_CMD)
all: library
	$(MAKE_CMD)
library:
	mkdir -p _objs
	$(CC) $(LIBS) -c -fPIC -o _objs/order.o exec/order.cpp
	$(CC) $(LIBS) -c -fPIC -o _objs/status.o exec/status.cpp
	$(CC) $(LIBS) -c -fPIC -o _objs/config.o exec/config.cpp
	$(CC) $(LIBS) -c -fPIC -o _objs/client.o exec/client.cpp
	$(CC) $(LIBS) -shared -o _objs/libalpaca.so _objs/client.o _objs/config.o _objs/order.o _objs/status.o
	sudo mv _objs/libalpaca.so /usr/local/lib
clean:
	rm -rf _objs
	rm -f $(TARGET)
	rm -rf $(TARGET).dSYM
