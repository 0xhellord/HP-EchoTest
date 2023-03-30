CC :=g++
CFLAGS := -l:libhpsocket.a -lpthread
TARGET := RPCTest

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
SRCS := $(wildcard *.cpp)
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

all: clean $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) 
%.o: %.cpp
	$(CC) -O3 -c $<  $(CFLAGS) 
clean:
	rm -rf $(TARGET) *.o

.PHONY: all clean
