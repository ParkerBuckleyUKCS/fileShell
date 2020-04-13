OBJECTS = scanner.o parser.o eventHandler.o prompter.o socketHandler.o main.cpp 
HEADERS = scanner.h parser.h eventHandler.h prompter.h socketHanlder.h

SOBJECTS = socketHandler.o server.cpp
STARGET = fileServer

CC = g++
CFLAGS = -g -Wall -std=c++17 -O3
TARGET = fileClient

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $^

$(STARGET): $(SOBJECTS)
	$(CC) $(CFLAGS) -o $(STARGET) $^

clean:
	rm -i *.o

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
