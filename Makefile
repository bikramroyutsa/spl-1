CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -I /opt/homebrew/include/eigen3 -I include
SRC = $(wildcard src/*.cpp)
TARGET = bin/main

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf bin *.o
