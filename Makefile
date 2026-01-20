CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I./includes/


TARGET = trie
SRC = main.cpp src/Trie.cpp src/TrieCLI.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
