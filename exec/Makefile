CC = g++
CFLAGS = -std=c++17 -w -g -lssl -lcrypto -lglog

TARGET = main
SOURCES := *.cpp

MAKE_CMD = $(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

all:
	$(MAKE_CMD)
clean:
	rm $(TARGET)
	rm -r $(TARGET).dSYM
