CXX = g++
CXXFLAGS = -O3 -Wall -std=c++11
TARGET = bin_packing
SRC = bin_packing.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# inject the instance.txt file automatically
run: $(TARGET)
	./$(TARGET) 2 < instance.txt

clean:
	rm -f $(TARGET)
