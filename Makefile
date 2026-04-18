CXX = g++
CXXFLAGS = -std=c++17 -g -Wall

TARGET = solution
SRC = solution.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) game_output.csv game_input.csv

test: $(TARGET)
	pytest test_game.py
