CC=g++
CFLAGS=-c -std=c++11 -g -Iinclude/ -DUSE_STANDALONE_ASIO
LDFLAGS=-pthread -lssl -lcrypto
WFLAGS=-Wall -Wextra -Wpedantic
SOURCES=src/main.cpp src/discord.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=a.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(IFLAGS) $(CFLAGS) $(WFLAGS) $< -o $@

clean:
	rm src/*.o
	rm $(EXECUTABLE)
