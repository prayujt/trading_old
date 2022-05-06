CC = g++
CFLAGS  = -std=c++11 -w -g -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -lmongocxx -lbsoncxx

TARGET = main
SOURCES := main.cpp
all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)
clean:
	rm $(TARGET)
	rm -r $(TARGET).dSYM
