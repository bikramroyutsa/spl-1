CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -I /opt/homebrew/include/eigen3
SRC = main.cpp parser.cpp occurence.cpp
TARGET = main

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o
